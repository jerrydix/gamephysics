#ifndef OPENPROJECTSIMULATOR_h
#define OPENPROJECTSIMULATOR_h
#include "Simulator.h"

// springsystems
struct Spring {
	int a, b;
	float initial_length, current_length;
	Vec3 current_force_from_a_to_b;
};

struct Masspoint {
	Vec3 position;
	Vec3 velocity;
	bool fixed;
};

class OpenProjectSimulator:public Simulator{
public:
	// Construtors
	OpenProjectSimulator();
	
	// UI Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void onClick(int x, int y);
	void onMouse(int x, int y);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void simulateSpringStepEuler(float timeStep);
	void simulateSpringStepMidpoint(float timeStep);


	// Specific Functions
		


private:

	// UI Attributes
	Vec3 m_externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;

	bool m_mouseMovement;

	// springs
	float m_fMass;
	float m_fStiffness;
	float m_fDamping;

	vector<Masspoint> masspoints;
	vector<Spring> springs;


	

};
#endif