#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "Primitive.h"

#ifdef _DEBUG
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics3D::ModulePhysics3D(bool start_enabled) : Module(start_enabled)
{
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);

	// Big rectangle as ground
	{
		btCollisionShape* colShape = new btBoxShape(btVector3(200.0f, 2.0f, 200.0f));

		mat4x4 glMatrix = IdentityMatrix;
		glMatrix.translate(0.f, -2.f, 0.f);
		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(&glMatrix);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	// TODO 8: Detect collisions:
	// - Get world dispatcher
	// - Iterate all manifolds
	// - Check we have more than 0 contacts
	// - If we have contacts, get both PhysBody3D's from userpointers
	// - Make sure both PhysBodies exist!
	// - Call "OnCollision" function on all listeners from both bodies

	for (int i = 0; i < world->getDispatcher()->getNumManifolds(); i++)							//Loop that iterates all manifolds present in the world. .getNumManifolds() returns the total amount of manifolds currently in the world.
	{
		btPersistentManifold* manifold = world->getDispatcher()->getManifoldByIndexInternal(i);	//Gets a manifold by its internal index number. The index given to the .getManifoldByIndexInternal() will be the index of the loop.

		if (manifold->getNumContacts() != 0)													//Gets the amount of contacts the manifold currently being iterated has. If there are more then 0 contacts.
		{
			PhysBody3D* PB1 = (PhysBody3D*)manifold->getBody0()->getUserPointer();				//Gets the body's pointer of the Body0 that the manifold has detected to have had a contact.
			PhysBody3D* PB2 = (PhysBody3D*)manifold->getBody1()->getUserPointer();				//Gets the body's pointer of the Body1 that the manifold has detected to have had a contact.

			if (PB1 != nullptr && PB2 != nullptr)												//If both PhysBody3D* pointers exist.
			{
				for (int n = 0; n < PB1->collision_listeners.Count(); n++)						//Loop that iterates all listeners of PB1 (colliding body).
				{
					PB1->collision_listeners[n]->OnCollision(PB1, PB2);							//Calls the OnCollision() method for the collision_listener currently being iterated.
				}

				for (int n = 0; n < PB2->collision_listeners.Count(); n++)						//Loop that iterates all listeners of PB1 (colliding body).
				{
					PB2->collision_listeners[n]->OnCollision(PB2, PB1);							//Calls the OnCollision() method for the collision_listener currently being iterated.
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->debug == true)
		world->debugDrawWorld();

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	delete world;

	return true;
}

void ModulePhysics3D::AddBodyToWorld(btRigidBody * body)
{
	world->addRigidBody(body);
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}

//void OnCollision(PhysBody3D* PB1, PhysBody3D* PB2)
//{
//
//}