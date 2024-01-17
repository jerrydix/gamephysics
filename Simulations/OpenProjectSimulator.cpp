#include "OpenProjectSimulator.h"

OpenProjectSimulator::OpenProjectSimulator()
{
	m_iTestCase = 0;

	m_externalForce = Vec3(0, -9.81, 0);
	m_mouseMovement = false;

	// springs
	this->m_fDamping    = 0.0f;
	this->m_fMass       = 1.0f;
	this->m_fStiffness  = 1.0f;

}

const char* OpenProjectSimulator::getTestCasesStr()
{
	return "Demo1,Demo2,Demo3,Demo4";
}

void OpenProjectSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;
	
}

void OpenProjectSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void OpenProjectSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	
}

void OpenProjectSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	m_trackmouse.x = m_trackmouse.y = 0;
}

void OpenProjectSimulator::externalForcesCalculations(float timeElapsed)
{
	
}


void OpenProjectSimulator::simulateTimestep(float timeStep)
{

}

void OpenProjectSimulator::simulateSpringStepEuler(float timeStep)
{
	 float inverse_mass = 1.f / this->m_fMass; // Since this value might just change in the UI, we need to reevaluate it per frame :(

    // @@Speed: Maybe precalculate some values, e.g. dt * inverse_mass
    
        //
        // Update all the springs
        //

        for(int i = 0; i < this->springs.size(); ++i) {
            Spring & spring = this->springs[i];
            Vec3 direction = this->masspoints[spring.a].position - this->masspoints[spring.b].position;
            spring.current_length = norm(direction);
            spring.current_force_from_a_to_b = -this->m_fStiffness * (spring.current_length - spring.initial_length) * (direction / spring.current_length);
        }

        //
        // Integrate all mass positions based on the previous' frame velocity.
        //

        for(int i = 0; i < this->masspoints.size(); ++i) {
            Masspoint & masspoint = this->masspoints[i];
            if(masspoint.fixed) continue;

            masspoint.position += timeStep * masspoint.velocity;
        }

        //
        // Update all mass velocities based on the spring forces
        //

        for(int i = 0; i < this->springs.size(); ++i) {
            Spring & spring = this->springs[i];
            Masspoint & a = this->masspoints[spring.a], & b = this->masspoints[spring.b];
            if(!a.fixed) a.velocity += timeStep * inverse_mass * (spring.current_force_from_a_to_b - a.velocity * this->m_fDamping);            
            if(!b.fixed) b.velocity -= timeStep * inverse_mass * (spring.current_force_from_a_to_b + b.velocity * this->m_fDamping);
        }
        
}

void OpenProjectSimulator::simulateSpringStepMidpoint(float timeStep)
{
	 float inverse_mass = 1.f / this->m_fMass; // Since this value might just change in the UI, we need to reevaluate it per frame :(

    // @@Speed: Maybe precalculate some values, e.g. dt * inverse_mass

   
        Vec3 temp_positions[masspoints.size()];
        Vec3 temp_velocities[masspoints.size()];
        float dt_2 = 0.5f * timeStep;
        
        //
        // Calculate x(t + h/2) based on v(t) for all mass points
        //
        
        for(int i = 0; i < this->masspoints.size(); ++i) {
            Masspoint & masspoint = this->masspoints[i];
            if(masspoint.fixed) {
                temp_velocities[i] = Vec3(0, 0, 0);
                temp_positions[i]  = masspoint.position;
                continue;
            }

            temp_positions[i]  = masspoint.position + dt_2 * masspoint.velocity;
            temp_velocities[i] = masspoint.velocity;
        }

        //
        // Calculate a(t) for all springs
        // Calculate v(t + h/2) based on a(t) of all springs for all masspoints
        //

        for(int i = 0; i < this->springs.size(); ++i) {
            Spring & spring = this->springs[i];
            Masspoint & a = this->masspoints[spring.a], & b = this->masspoints[spring.b];

            // Calculate a(t) for all springs, based on x(t) and v(t)

            Vec3 direction = a.position - b.position;
            spring.current_length = norm(direction);
            spring.current_force_from_a_to_b = -this->m_fStiffness * (spring.current_length - spring.initial_length) * (direction / spring.current_length);
            
            // Calculate v(t + h/2) based on the spring forces for all masspoints
            
            if(!a.fixed) temp_velocities[spring.a] += dt_2 * inverse_mass * (spring.current_force_from_a_to_b - temp_velocities[spring.a] * this->m_fDamping);
            if(!b.fixed) temp_velocities[spring.b] -= dt_2 * inverse_mass * (spring.current_force_from_a_to_b + temp_velocities[spring.b] * this->m_fDamping);
        }

        //
        // Calculate x(t + h) using the temp velocity
        //

        for(int i = 0; i < this->masspoints.size(); ++i) {
            Masspoint & masspoint = this->masspoints[i];
            masspoint.position += timeStep * temp_velocities[i];
        }

        //
        // Calculate a(t + h/2) based on the temporary positions and velocities for all springs
        // Calculate v(t + h) based on the spring forces for all masspoints
        //

        for(int i = 0; i < this->springs.size(); ++i) {
            // Calculate a(t + h/2) based on the temporary positions and velocities for all springs
            
            Spring & spring = this->springs[i];
            Vec3 direction = (temp_positions[spring.a] + dt_2 * temp_velocities[spring.a]) - (temp_positions[spring.b] + dt_2 * temp_velocities[spring.b]);
            float current_length = norm(direction);
            spring.current_force_from_a_to_b = -this->m_fStiffness * (current_length - spring.initial_length) * (direction / current_length);

            // Calculate v(t + h) based on the spring forces for all masspoints

            Masspoint & a = this->masspoints[spring.a], & b = this->masspoints[spring.b];

            if(!a.fixed) a.velocity += timeStep * inverse_mass * (spring.current_force_from_a_to_b - a.velocity * this->m_fDamping);            
            if(!b.fixed) b.velocity -= timeStep * inverse_mass * (spring.current_force_from_a_to_b + b.velocity * this->m_fDamping);
        }
  
}

void OpenProjectSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void OpenProjectSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = m_trackmouse.x;
	m_oldtrackmouse.y = m_trackmouse.y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
