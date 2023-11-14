#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = 0;
	m_fMass = 10;
	m_fStiffness = 40;
	m_fDamping = 0.0;
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
	addSpring(0, 1, 2);
}

const char* MassSpringSystemSimulator::getTestCasesStr()
{
	return "Demo1,Demo2,Demo3,Demo4";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;
}

void MassSpringSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	m_vPoints.clear();
	m_vSprings.clear();
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
	addSpring(0, 1, 2);
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	// Draw the mass points
	for (int i = 0; i < m_vPoints.size(); i++)
	{
		DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, Vec3(0, 0, 0));
		DUC->drawSphere(m_vPoints.at(i).position, Vec3(0.1, 0.1, 0.1));
	}

	// Draw the springs
	for (int i = 0; i < m_vSprings.size(); i++)
	{
		DUC->beginLine();
		DUC->drawLine(m_vPoints.at(m_vSprings.at(i).masspoint1).position, Vec3(1, 1, 1), m_vPoints.at(m_vSprings.at(i).masspoint2).position, Vec3(1, 1, 1));
		DUC->endLine();
	}
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case 0:
		cout << "One step!\n";
		break;
	case 1:
		cout << "Euler!\n";
		break;
	case 2:
		cout << "Midpoint!\n";
		break;
	default:
		cout << "Complex!\n";
		break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	switch (m_iTestCase) {
	case 0: {
		// One step
		for (int i = 0; i < m_vSprings.size(); i++)
		{
			//point 1
			MassPoint& point1 = m_vPoints.at(m_vSprings.at(i).masspoint1);

			//point 2
			MassPoint& point2 = m_vPoints.at(m_vSprings.at(i).masspoint2);

			//reset point attributes
			point1.position = Vec3(0, 0, 0);
			point1.velocity = Vec3(-1, 0, 0);
			point2.position = Vec3(0, 2, 0);
			point2.velocity = Vec3(1, 0, 0);

			if (!point1.isFixed) {
				cout << point1.position << endl;
				Vec3 D = point1.position - point2.position;
				float DLength = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
				Vec3 normalizedD = D / DLength;
				Vec3 p1_force = -m_fStiffness * (DLength - m_vSprings.at(i).initialLength) * normalizedD;
				Vec3 acceleration = p1_force / m_fMass;

				point1.velocity += timeStep * acceleration;
				point1.position += timeStep * point1.velocity;
				cout << point1.position << endl;
			}

			if (!point2.isFixed) {
				Vec3 D = point2.position - point1.position;
				float DLength = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
				Vec3 normalizedD = D / DLength;
				Vec3 p2_force = -m_fStiffness * (DLength - m_vSprings.at(i).initialLength) * normalizedD;
				Vec3 acceleration = p2_force / m_fMass;

				point2.velocity += timeStep * acceleration;
				point2.position += timeStep * point2.velocity;
			}


			
		}
		break;
	}
	case 1: {
		for (int i = 0; i < m_vSprings.size(); i++)
		{
			//point 1
			MassPoint& point1 = m_vPoints.at(m_vSprings.at(i).masspoint1);

			//point 2
			MassPoint& point2 = m_vPoints.at(m_vSprings.at(i).masspoint2);

			if (!point1.isFixed) {
				cout << point1.position << endl;
				Vec3 D = point1.position - point2.position;
				float DLength = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
				Vec3 normalizedD = D / DLength;
				Vec3 p1_force = -m_fStiffness * (DLength - m_vSprings.at(i).initialLength) * normalizedD;
				Vec3 acceleration = p1_force / m_fMass;

				point1.velocity += timeStep * acceleration;
				point1.position += timeStep * point1.velocity;
				cout << point1.position << endl;
			}

			if (!point2.isFixed) {
				Vec3 D = point2.position - point1.position;
				float DLength = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
				Vec3 normalizedD = D / DLength;
				Vec3 p2_force = -m_fStiffness * (DLength - m_vSprings.at(i).initialLength) * normalizedD;
				Vec3 acceleration = p2_force / m_fMass;

				point2.velocity += timeStep * acceleration;
				point2.position += timeStep * point2.velocity;
			}



		}
		break;
	}
	case 2: {
		for (int i = 0; i < m_vSprings.size(); i++)
		{
			//point 1
			MassPoint& point1 = m_vPoints.at(m_vSprings.at(i).masspoint1);

			//point 2
			MassPoint& point2 = m_vPoints.at(m_vSprings.at(i).masspoint2);

			Vec3 midStepPos1 = point1.position + 1 / 2 * timeStep * point1.velocity;
			Vec3 midStepPos2 = point2.position + 1 / 2 * timeStep * point2.velocity;


			if (!point1.isFixed) {

				//acceleration at oldPos
				Vec3 D = point1.position - point2.position;
				float DLength = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
				Vec3 normalizedD = D / DLength;
				Vec3 p_force = -m_fStiffness * (DLength - m_vSprings.at(i).initialLength) * normalizedD;
				Vec3 acceleration = p_force / m_fMass;

				//acceleration at midstep
				Vec3 m_D = midStepPos1 - midStepPos2;
				float m_DLength = sqrt(m_D.x * m_D.x + m_D.y * m_D.y + m_D.z * m_D.z);
				Vec3 m_normalizedD = m_D / m_DLength;
				Vec3 m_p_force = -m_fStiffness * (m_DLength - m_vSprings.at(i).initialLength) * m_normalizedD;
				Vec3 m_acceleration = m_p_force / m_fMass;

				Vec3 midStepVel = point1.velocity + 1/2 * timeStep * acceleration;

				//newPos
				point1.position += timeStep * midStepVel;

				//newVel
				point1.velocity += timeStep * m_acceleration;

			

				
			}

			if (!point2.isFixed) {
				
				//acceleration at oldPos
				Vec3 D = point2.position - point1.position;
				float DLength = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
				Vec3 normalizedD = D / DLength;
				Vec3 p_force = -m_fStiffness * (DLength - m_vSprings.at(i).initialLength) * normalizedD;
				Vec3 acceleration = p_force / m_fMass;

				//acceleration at midstep
				Vec3 m_D = midStepPos2 - midStepPos1;
				float m_DLength = sqrt(m_D.x * m_D.x + m_D.y * m_D.y + m_D.z * m_D.z);
				Vec3 m_normalizedD = m_D / m_DLength;
				Vec3 m_p_force = -m_fStiffness * (m_DLength - m_vSprings.at(i).initialLength) * m_normalizedD;
				Vec3 m_acceleration = m_p_force / m_fMass;

				Vec3 midStepVel = point2.velocity + 1 / 2 * timeStep * acceleration;

				//newPos
				point2.position += timeStep * midStepVel;

				//newVel
				point2.velocity += timeStep * m_acceleration;
			}



		}
		cout << "TEST" << endl;
		break;
	}
	}

}

void MassSpringSystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed)
{
	m_vPoints.push_back(MassPoint{ position, Velocity, isFixed });
	return m_vPoints.size() - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
	m_vSprings.push_back(Spring{ masspoint1, masspoint2, initialLength });
}

int MassSpringSystemSimulator::getNumberOfMassPoints()
{
	return m_vPoints.size();
}

int MassSpringSystemSimulator::getNumberOfSprings()
{
	return m_vSprings.size();
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
{
	return m_vPoints.at(index).position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
{
	return m_vPoints.at(index).velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
	m_externalForce = force;
}

void MassSpringSystemSimulator::setMass(float mass)
{
	m_fMass = mass;
}

void MassSpringSystemSimulator::setStiffness(float stiffness)
{
	m_fStiffness = stiffness;
}

void MassSpringSystemSimulator::setDampingFactor(float damping)
{
	m_fDamping = damping;
}