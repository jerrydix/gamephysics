#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = 0;
	m_fMass = 10;
	m_fStiffness = 40;
	m_fDamping = 0.0;
	m_iIntegrator = MIDPOINT;
	m_externalForce = Vec3(0, 0, 0);
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
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	// Draw the mass points
	for (int i = 0; i < m_vPoints.size(); i++)
	{
		DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, Vec3(0, 0, 0));
		DUC->drawSphere(m_vPoints[i].position, Vec3(0.1, 0.1, 0.1));
	}

	// Draw the springs
	for (int i = 0; i < m_vSprings.size(); i++)
	{
		DUC->beginLine();
		DUC->drawLine(m_vPoints[m_vSprings[i].masspoint1].position, Vec3(1, 1, 1), m_vPoints[m_vSprings[i].masspoint2].position, Vec3(1, 1, 1));
		DUC->endLine();
	}
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{

}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	switch (m_iIntegrator)
	{
	case EULER:
	{
		for (int i = 0; i < m_vPoints.size(); i++)
		{
			if (!m_vPoints[i].isFixed)
			{
				m_vPoints[i].position += timeStep * m_vPoints[i].velocity;
				m_vPoints[i].velocity += timeStep * (m_vPoints[i].force / m_fMass);
			}
		}
		break;
	}
	case LEAPFROG:
	{
		for (int i = 0; i < m_vPoints.size(); i++)
		{
			if (!m_vPoints[i].isFixed)
			{
				m_vPoints[i].velocity += timeStep * (m_vPoints[i].force / m_fMass);
				m_vPoints[i].position += timeStep * m_vPoints[i].velocity;
			}
		}
		break;
	}
	case MIDPOINT:
	{
		for (int i = 0; i < m_vPoints.size(); i++)
		{
			if (!m_vPoints[i].isFixed)
			{
				Vec3 midVelocity = m_vPoints[i].velocity + timeStep * (m_vPoints[i].force / m_fMass);
				Vec3 midPosition = m_vPoints[i].position + timeStep * midVelocity;
				m_vPoints[i].velocity += timeStep * (m_vPoints[i].force / m_fMass);
				m_vPoints[i].position += timeStep * midVelocity;
			}
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