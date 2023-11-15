#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = 0;
	m_fMass = 10;
	m_fStiffness = 40;
	m_fDamping = 0.0;

	g_method = 0;
	m_externalForce = Vec3(0, -9.81, 0);
}

const char* MassSpringSystemSimulator::getTestCasesStr()
{
	return "Demo1,Demo2,Demo3,Demo4";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;
	if (m_iTestCase == 3) {
		TwType MASS_METHOD = TwDefineEnumFromString("Integrator","Euler,Midpoint");
		TwAddVarRW(DUC->g_pTweakBar, "Integrator", MASS_METHOD, &g_method, "");
	}
}

void MassSpringSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	m_vPoints.clear();
	m_vSprings.clear();
	//TODO copy code from notifyCaseChanged here
	switch (m_iTestCase)
	{
	case 0:
		cout << "One step!\n";
		addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(0, 1, 2);
		break;
	case 1:
		cout << "Euler!\n";
		addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(0, 1, 2);
		break;
	case 2:
		cout << "Midpoint!\n";
		addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(0, 1, 2);
		break;
	default:
		cout << "Complex!\n";
		for (size_t i = 0; i < 10; i++)
		{
			addMassPoint(Vec3(0 + i, 0, 0), Vec3(-1 + i, 0, 0), false);
			addMassPoint(Vec3(0, 2 + i, 0), Vec3(1 + i, 0, 0), false);
			addSpring(i, i + 1, 2);
		}
		break;
	}
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	// Draw the mass points
	for (int i = 0; i < m_vPoints.size(); i++)
	{
		DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, Vec3(0, 0, 0));
		DUC->drawSphere(m_vPoints.at(i).position, Vec3(0.05, 0.05, 0.05));
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
		m_vPoints.clear();
		m_vSprings.clear();
		addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(0, 1, 2);
		break;
	case 1:
		cout << "Euler!\n";
		m_vPoints.clear();
		m_vSprings.clear();
		addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(0, 1, 2);
		break;
	case 2:
		cout << "Midpoint!\n";
		m_vPoints.clear();
		m_vSprings.clear();
		addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(0, 1, 2);
		break;
	default:
		cout << "Complex!\n";
		m_vPoints.clear();
		m_vSprings.clear();

		//weird attached kraken object
		//0-7

		addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0.25, 0, 1), Vec3(1, 0, 0), false);
		addMassPoint(Vec3(0, 0.25, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0.25, 0.25, 1), Vec3(1, 0, 0), false);
		addMassPoint(Vec3(0, 0.5, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0.25, 0.5, 1), Vec3(1, 0, 0), false);
		addMassPoint(Vec3(0, 0.75, 0), Vec3(-1, 0, 0), false);
		addMassPoint(Vec3(0.15, 2, 1), Vec3(0, 0, 0), true);

		addSpring(0, 1, 0.5);
		addSpring(0, 3, 0.5);
		addSpring(2, 3, 0.5);
		addSpring(0, 2, 0.5);
		addSpring(1, 3, 0.5);
		addSpring(1, 5, 0.5);
		addSpring(3, 5, 0.5);
		addSpring(2, 4, 0.5);
		addSpring(3, 4, 0.5);
		addSpring(5, 6, 0.5);
		addSpring(4, 6, 0.5);
		addSpring(5, 7, 0.5);
		addSpring(6, 7, 0.5);


		//cube //0.866 diagonal
		addMassPoint(Vec3(3, -0.9, 0), Vec3(0, 0, 0), false); //8
		addMassPoint(Vec3(3.5, -0.9, 0), Vec3(0, 0, 0), false); //9

		addMassPoint(Vec3(3, -0.9, 0.5), Vec3(0, 0, 0), false); //10
		addMassPoint(Vec3(3.5, -0.9, 0.5), Vec3(0, 0, 0), false); //11

		addMassPoint(Vec3(3, -0.4, 0), Vec3(0, 0, 0), false); //12
		addMassPoint(Vec3(3.5, -0.4, 0), Vec3(0, 0, 0), false); //13

		addMassPoint(Vec3(3, -0.4, 0.5), Vec3(0, 0, 0), false); //14
		addMassPoint(Vec3(3.5, -0.4, 0.5), Vec3(0, 0, 0), false); //15

		addSpring(8, 9, 0.5);
		addSpring(8, 10, 0.5);
		addSpring(8, 12, 0.5);
		addSpring(9, 11, 0.5);
		addSpring(9, 13, 0.5);
		addSpring(10, 14, 0.5);
		addSpring(10, 11, 0.5);
		addSpring(11, 15, 0.5);
		addSpring(12, 13, 0.5);
		addSpring(12, 14, 0.5);
		addSpring(13, 15, 0.5);
		addSpring(14, 15, 0.5);

		addSpring(8, 13, 0.3);
		addSpring(9, 12, 0.3);
		addSpring(9, 15, 0.3);
		addSpring(13, 11, 0.3);
		addSpring(13, 11, 0.3);
		addSpring(10, 12, 0.3);
		addSpring(8, 14, 0.3);
		addSpring(11, 14, 0.3);
		addSpring(10, 15, 0.3);
		addSpring(13, 14, 0.3);
		addSpring(12, 15, 0.3);
		addSpring(9, 10, 0.3);
		addSpring(8, 12, 0.3);


		addSpring(8, 15, 0.4);
		addSpring(9, 14, 0.4);
		addSpring(10, 13, 0.4);
		addSpring(11, 12, 0.4);

		break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{

}

void MassSpringSystemSimulator::performEuler(float timeStep) {
	for (int i = 0; i < m_vSprings.size(); i++)
	{
		//point 1
		MassPoint& point1 = m_vPoints.at(m_vSprings.at(i).masspoint1);

		//point 2
		MassPoint& point2 = m_vPoints.at(m_vSprings.at(i).masspoint2);

		if (!point1.isFixed) {

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
}

void MassSpringSystemSimulator::performMidpoint(float timeStep) {
	for (int i = 0; i < m_vSprings.size(); i++)
	{
		//point 1
		MassPoint& point1 = m_vPoints.at(m_vSprings.at(i).masspoint1);

		//point 2
		MassPoint& point2 = m_vPoints.at(m_vSprings.at(i).masspoint2);


		if (!point1.isFixed) {

			Vec3 midStepPos1 = point1.position + 1 / 2 * timeStep * point1.velocity;
			Vec3 midStepPos2 = point2.position + 1 / 2 * timeStep * point2.velocity;

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

			Vec3 midStepVel = point1.velocity + 1 / 2 * timeStep * acceleration;

			//newPos
			point1.position += timeStep * midStepVel;

			//newVel
			point1.velocity += timeStep * m_acceleration;

		}

		if (!point2.isFixed) {

			Vec3 midStepPos1 = point1.position + 1 / 2 * timeStep * point1.velocity;
			Vec3 midStepPos2 = point2.position + 1 / 2 * timeStep * point2.velocity;

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
				Vec3 D = point1.position - point2.position;
				float DLength = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
				Vec3 normalizedD = D / DLength;
				Vec3 p1_force = -m_fStiffness * (DLength - m_vSprings.at(i).initialLength) * normalizedD;
				Vec3 acceleration = p1_force / m_fMass;

				point1.velocity += 0.1 * acceleration;
				point1.position += 0.1 * point1.velocity;
				cout << point1.position << endl;
			}

			if (!point2.isFixed) {
				Vec3 D = point2.position - point1.position;
				float DLength = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
				Vec3 normalizedD = D / DLength;
				Vec3 p2_force = -m_fStiffness * (DLength - m_vSprings.at(i).initialLength) * normalizedD;
				Vec3 acceleration = p2_force / m_fMass;

				point2.velocity += 0.1 * acceleration;
				point2.position += 0.1 * point2.velocity;
			}
			
		}
		break;
	}
	case 1: {
		performEuler(0.005);
		break;
	}
	case 2: {
		performMidpoint(0.005);
		break;
	}
	case 3: {
		for (size_t i = 0; i < m_vPoints.size(); i++)
		{
			if (m_externalForce.y != 0) {
				m_vPoints.at(i).velocity += timeStep * m_externalForce;
			}
			if (m_vPoints.at(i).position.y <= -0.95) {
				m_vPoints.at(i).velocity = m_vPoints.at(i).velocity - 2 * (dot(m_vPoints.at(i).velocity, Vec3(0, 1, 0))) * Vec3(0, 1, 0);
			}
		}
		
		if (g_method == 0) {
			performEuler(timeStep);
		}
		else {
			performMidpoint(timeStep);
		}
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