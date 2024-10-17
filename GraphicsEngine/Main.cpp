#pragma once
#define GLM_SWIZZLE
#include "Main.h"
#include <typeinfo>
#include "Assets/CollGrid.h"


//GLuint vao;
//GLuint vbo;

bool focused = true;
int windowWidth = 1000;
int windowHeight = 900;
float FOV = 45;
float nearPlane = 0.05;
float farPlane = 500.0f;

bool splashScreen = true;

//delta time
double oldTime;
double currentTime;

#pragma region View Matrix Variables

//Define base axis
glm::vec3 targetPos = glm::vec3(0, 0, 0);
glm::vec3 cameraPos = glm::vec3(0, 10, 0);
glm::vec3 upPos = glm::vec3(0, 1, 0);
glm::vec3 right = glm::vec3(1, 0, 0);
glm::mat4 ViewRotateX;

Camera* camera = new Camera(cameraPos, upPos);


//store the original position of the mouse
//this division sets it to the centre of the screen 
float prevX = windowWidth / 2.0f;
float prevY = windowHeight / 2.0f;
#pragma endregion


#pragma region Functions

void showMessage(char message)
{
    std::cout << message << std::endl;
}

void ShaderCompileChecker(GLuint shader)
{
    GLint result = GL_FALSE;
    int logLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;

    //remove the comments on the code below to make program crash with error
    //if (result == GL_FALSE)
    //throw std::exception("Shader error! Check the console output to see where the error in the shader code is!");
}

void Render(ObjShape obj, Shader shader, glm::vec4 rot = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) 
{
    //shader.changeTransform(obj.currPos, obj.scale);
    shader.changeTransform(obj.currPos, obj.scale, glm::vec3(rot.x, rot.y, rot.z));
    obj.Draw();
}

//input event

void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //make sure that user can't move during introduction
    if (!splashScreen) 
    {
        //camera.Input(key);

        if (action == GLFW_PRESS) //If a key is pressed
        {
            Object::keyInput[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            Object::keyInput[key] = false;
        }
    }
    else 
    {
        //stop the splash screen when any input is detected
        splashScreen = false;
    }

    if (key == GLFW_KEY_ESCAPE) 
    {
        if (focused)
        {
            glfwDestroyWindow(window);
            glfwTerminate();
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    if (key == GLFW_KEY_SPACE) 
    {
        std::cout << "Pause" << std::endl;
    }
    
    if (key == GLFW_KEY_T) 
    {
        std::cout << "Position is:" << glm::to_string(camera->position) << std::endl;
    }

    #pragma region Change Car

    if (key == GLFW_KEY_1) 
    {
        CurrentPlayer->GetScript<PlayerScript>()->active = false;

        for (Object* obj : GameObjects) 
        {
            if (obj->name == "Player") 
            {
                obj->GetScript<PlayerScript>()->active = true;
                CurrentPlayer = obj;
                break;
            }
        }
    }
    
    if (key == GLFW_KEY_2) 
    {
        CurrentPlayer->GetScript<PlayerScript>()->active = false;

        for (Object* obj : GameObjects) 
        {
            if (obj->name == "Player2") 
            {
                obj->GetScript<PlayerScript>()->active = true;
                CurrentPlayer = obj;
                break;
            }
        }
    }

    #pragma endregion
}

void MouseInput(GLFWwindow* window, double XIn, double YIn)
{
    if (!splashScreen)
    {
        float xpos = (float)(XIn);
        float ypos = (float)(YIn);

        float X = xpos - prevX;
        float Y = prevY - ypos; // reversed since y-coordinates go from bottom to top

        camera->MouseInput(X, Y);

        prevX = xpos;
        prevY = ypos;
    }
}


#pragma endregion


int main(int argc, char** argv)
{
     
	glfwInit();
	//Define GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #pragma region GL window Setup

	    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Ben's Physics Simulation", NULL, NULL);
	    //GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Ben's Graphics Simulation", glfwGetPrimaryMonitor(), NULL);
    
        if (!window)
        {
            std::cout << "ERROR: No window was created!" << std::endl;
            return -1;
        }

        //assign the new windo to the GL's context

        glfwMakeContextCurrent(window);

        //initiate 
        if (gl3wInit())
        {
            std::cout << "ERROR: Failed to initialize OpenGL" << std::endl;
            return -1;
        }

        //How often the buffer should swap - 1 = do everytime there is a screen update
        glfwSwapInterval(1);

        gl3wProcs.gl.Enable(GL_DEPTH_TEST);

        float aspectRatio = windowWidth / windowHeight;
        float verticalFov = glm::radians(FOV);
        float halfFov = glm::tan(verticalFov / 2.0);

    #pragma endregion

    #pragma region Setup Assets

        #pragma region shader setup

        //Create shader programs
        //Create them once so we can use less RAM
        Shader cubeMapShader("Assets/Shaders/Skybox/VertexShader.glsl", "Assets/Shaders/Skybox/FragShader.glsl");
        Shader multiShader("Assets/Shaders/LightVertShader.glsl", "Assets/Shaders/MultiLightFrag.glsl");
        Shader unlit("Assets/Shaders/VertexShader.glsl", "Assets/Shaders/FragShader.glsl");
        Shader colliderShader("Assets/Shaders/VertexShader.glsl", "Assets/Shaders/FragShader.glsl");

        multiShader.use();

        #pragma endregion
        
        #pragma region Textures

            Texture texture2("Assets/imageData/TestTexture.png", multiShader);
            Texture truckText("Assets/imageData/TruckMat.png", multiShader);
            //Texture rockTexture("Assets/imageData/Rock.png", multiShader);
            //Texture IdolTexture("Assets/imageData/Idol/base.png", multiShader);
            //Texture leafTexture("Assets/imageData/leaf.jpg", multiShader);
            //Texture bark("Assets/imageData/barkDark.png", multiShader);
            Texture floorTexture("Assets/imageData/forestFloor.jpg", multiShader, "Assets/imageData/ForestNormal.png");
            Texture DirtTexture("Assets/imageData/dirt.jpg", multiShader);
            Texture waterText("Assets/imageData/waterImage.jpg", multiShader);
            Texture SplashText("Assets/Textures/SplashScreenImg.png", multiShader);
            Texture collTexture("Assets/imageData/TestTexture.png", colliderShader);
            Texture boostTexture("Assets/Textures/BoostTexture.png", multiShader);

            //building skybox texture
            std::vector<const char*> faces
            {
                "Assets/imageData/Cubemap/posx.jpg",
                "Assets/imageData/Cubemap/negx.jpg",
                "Assets/imageData/Cubemap/posy.jpg",
                "Assets/imageData/Cubemap/negy.jpg",
                "Assets/imageData/Cubemap/posz.jpg",
                "Assets/imageData/Cubemap/negz.jpg",
            };
            CubemapT CubemapText(faces, cubeMapShader);


        #pragma endregion

        #pragma region Hard coded shapes

            //skybox vertices
            GLfloat skyboxVertices[] = {
                // positions          
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                    1.0f,  1.0f, -1.0f,
                    1.0f,  1.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                    1.0f, -1.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                    1.0f, -1.0f,  1.0f
            };

            std::vector<float> skyBoxVerts =
            {
                -10.0f , -10.0f ,  10.0f,//0
                10.0f , -10.0f ,  10.0f,//1
                10.0f , -10.0f , -10.0f,//2
                -10.0f , -10.0f , -10.0f,//3
                -10.0f ,  10.0f ,  10.0f,//now we do top layer of cube //4
                10.0f ,  10.0f ,  10.0f,//5
                10.0f ,  10.0f , -10.0f,//6
                -10.0f ,  10.0f , -10.0f //7
            };

            //all the points for the cube vertices
            std::vector<VertexData> CubeData{
                { {0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f, -0.5f} },//starting furthest bottom corner and going around the bottom vertices//0
                { {1.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f, -0.5f} },//1
                { {1.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f,  0.5f} },//2
                { {0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f,  0.5f} },//3
                { {0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f, -0.5f} },//now we do top layer of cube //4
                { {1.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f, -0.5f} },//5
                { {1.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f,  0.5f} },//6
                { {0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f,  0.5f} }//7
            };

            std::vector<VertexData> squareData{
                { {-0.5f,   0, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f, -0.5f}, {0 ,0}},
                { { 0.5f,   0, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f, -0.5f}, {1, 0} },
                { { 0.5f,   0,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f,  0.5f}, {1, 1} },
                { {-0.5f,   0,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f,  0.5f}, {0, 1}}
            };

            std::vector<VertexData> splashVerts{
                { {-0.5f, -0.5,   0}  , {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f, -0.5f}, {0 ,0}},
                { { 0.5f, -0.5,   0}  , {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f, -0.5f}, {1, 0} },
                { { 0.5f,  0.5,   0}  , {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f,  0.5f}, {1, 1} },
                { {-0.5f,  0.5,   0}  , {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f,  0.5f}, {0, 1}}
            };

            //std::vector<VertexData> CubeData2{
            //    { {-1.5f, -0.5f, -0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f, -0.5f} },//starting furthest bottom corner and going around the bottom vertices//0
            //    { {-0.5f, -0.5f, -0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f, -0.5f} },//1
            //    { {-0.5f, -0.5f,  0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f,  0.5f} },//2
            //    { {-1.5f, -0.5f,  0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f,  0.5f} },//3
            //    { {-1.5f,  0.5f, -0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f, -0.5f} },//now we do top layer of cube //4
            //    { {-0.5f,  0.5f, -0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f, -0.5f} },//5
            //    { {-0.5f,  0.5f,  0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f,  0.5f} },//6
            //    { {-1.5f,  0.5f,  0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f,  0.5f} }//7
            //};

            //all the points for the cube vertices
            std::vector<unsigned int> cubeFaces
            {
                //top face
                4,7,6,
                6,5,4,

                //bottom face
                0,1,2,
                2,3,0,

                //front face
                2,3,7,
                7,6,2,

                //back face
                0,1,5,
                5,4,0,


                //right face
                1,2,6,
                6,5,1,

                //left face
                0,3,7,
                7,4,0
            };
            //all the points for the cube vertices
            std::vector<unsigned int> squareFaces
            {
                //top face
                0,3,2,
                2,1,0,
            };


        #pragma endregion

        #pragma region Meshes

            //import the object vertex data once and then reuse them, takes less ram
            //std::vector<VertexData> cubeObj = LoadOBJ("Assets/Models/cube1.obj");
            ObjShape cubeMesh = LoadOBJ("Assets/Models/cube1.obj");
            ObjShape cloudMesh = LoadOBJ("Assets/Models/Other/cloud.obj");

            //std::vector<VertexData> carMesh = LoadOBJ("Assets/Models/car/RaceCar.obj");
            ObjShape truckMesh = LoadOBJ("Assets/Models/car/Truck.obj");
            ObjShape carMesh = LoadOBJ("Assets/Models/car/RaceCar.obj");

            //std::vector<VertexData> trackMesh = LoadOBJ("Assets/Models/track/racetrack.obj");
            ObjShape trackMesh = LoadOBJ("Assets/Models/track/racetrack.obj");

            //std::vector<VertexData> rainObj = LoadOBJ("Assets/Models/other/RainPlane.obj");
            ObjShape planeMesh = LoadOBJ("Assets/Models/other/RainPlane.obj");
            ObjShape rain = planeMesh;

            //std::vector<VertexData> sphereMesh = LoadOBJ("Assets/Models/Sphere.obj");
            ObjShape sphereMesh = LoadOBJ("Assets/Models/Sphere.obj");
            //ObjShape colliderMesh = ObjShape(sphereMesh);
            ObjShape colliderMesh = LoadOBJ("Assets/Models/Sphere.obj");
            ObjShape boostMesh = LoadOBJ("Assets/Models/track/Boost.obj");
            //ObjShape cubeMesh = ObjShape(cubeObj);
            //ObjShape planeMesh = ObjShape(rainObj);

        #pragma endregion

    #pragma endregion
    
    #pragma region Object Creation

        #pragma region Cars

        //create Gameobjects
        
        Object* car = new Object(multiShader);
        CurrentPlayer = car;
        car->name = "Player";
        car->tag = Player;
        car->transform.pos = glm::vec3{ 0.0f, 1.5f, 0.0f };
        car->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        car->transform.scale = { 0.5, 0.5, 0.5 };
        car->objTexture = &texture2;

        //adding components to an object
        //and changing component data

        car->AddComponent(mesh, &carMesh);

        car->AddScript(new PlayerScript());
        car->GetScript<PlayerScript>()->moveSpeed = 50.0f;
        car->GetScript<PlayerScript>()->turnSpeed = 30.0f;

        car->AddComponent(rigidbody, 20.0f);
        car->GetComponent<Rigidbody>()->bounce = 0.5f;

        //For changing colliders
        //Comment the collider below, and uncomment another collider

        car->AddComponent(collider, Sphere, 0.5f, &multiShader, &colliderMesh);
        car->GetComponent<SphereCollider>()->originalPos.y = 0.25f;

        /*
        car->AddComponent(collider, Cube, 0.1f, &multiShader, &cubeMesh);
        car->GetComponent<CubeCollider>()->originalPos.y = 0.25f;
        car->GetComponent<CubeCollider>()->width = 0.5f;
        car->GetComponent<CubeCollider>()->height = 0.25f;
        car->GetComponent<CubeCollider>()->length = 0.5f;
        */
        /*
        car->AddComponent(collider, OBB, 0.1f, &multiShader, &cubeMesh);
        car->GetComponent<OBBCollider>()->originalPos.y = 0.25f;
        car->GetComponent<OBBCollider>()->width = 0.5f;
        car->GetComponent<OBBCollider>()->height = 0.25f;
        car->GetComponent<OBBCollider>()->length = 0.5f;
        */

        Object* car2 = new Object(multiShader);
        car2->name = "Player2";
        car2->tag = Player;
        car2->transform.pos = glm::vec3{ -10.0f, 1.5f, 0.0f };
        car2->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        car2->transform.scale = { 0.5, 0.5, 0.5 };
        car2->objTexture = &truckText;
           
        car2->AddComponent(mesh, &truckMesh);
           
        car2->AddScript(new PlayerScript());
        car2->GetScript<PlayerScript>()->active = false;
        car2->GetScript<PlayerScript>()->moveSpeed = 80.0f;
        car2->GetScript<PlayerScript>()->turnSpeed = 50.0f;
           
        car2->AddComponent(rigidbody, 40.0f);
        car2->GetComponent<Rigidbody>()->bounce = 0.5f;
        car2->GetComponent<Rigidbody>()->drag = 0.25f;
           
        car2->AddComponent(collider, Cube, 0.25f, &multiShader, &cubeMesh);
        car2->GetComponent<CubeCollider>()->originalPos.y = 0.25f;
        car2->GetComponent<CubeCollider>()->width = 0.5f;
        car2->GetComponent<CubeCollider>()->height = 0.25f;
        car2->GetComponent<CubeCollider>()->length = 0.5f;


        #pragma endregion

        Object* GroundCollider = new Object(multiShader);
        GroundCollider->name = "GROUND";
        GroundCollider->tag = Ground;

        GroundCollider->transform.pos = glm::vec3{ 0.0f, 0.0f, 0.0f };
        GroundCollider->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        GroundCollider->transform.scale = glm::vec3(1.0f);

        GroundCollider->AddComponent(particle, 100.0f);
        GroundCollider->GetComponent<Particle>()->bounce = 0.25f;
        GroundCollider->GetComponent<Particle>()->BaseDrag = 0.25f;
        GroundCollider->GetComponent<Particle>()->movable = false;
        GroundCollider->AddComponent(collider, Plane, 0.01f, &multiShader, &colliderMesh);
        GroundCollider->GetComponent<PlaneCollider>(planeCollider);


        Object* CubeColl = new Object(multiShader);
        CubeColl->name = "CubeColl";
        CubeColl->tag = Ground;

        CubeColl->transform.pos = glm::vec3{ -87.0f, 13.0f, 63.0f };
        //CubeColl->transform.pos = glm::vec3{ 0.0f, 0.0f, 10.0f};
        CubeColl->transform.rotation = glm::vec3(0, 0, 0.0f);
        CubeColl->transform.scale = glm::vec3(20.0f, 0.5f, 4.0f);
        CubeColl->AddComponent(mesh, &cubeMesh);

        CubeColl->AddComponent(rigidbody, 100.0f);
        CubeColl->GetComponent<Rigidbody>()->gravity = false;
        CubeColl->GetComponent<Rigidbody>()->bounce = 0.75f;
        CubeColl->GetComponent<Rigidbody>()->BaseDrag = 0.1f;
        CubeColl->GetComponent<Rigidbody>()->movable = false;

        /*
        CubeColl->AddComponent(collider, OBB, 0.05f, &multiShader, &cubeMesh);
        CubeColl->GetComponent<OBBCollider>()->width = 10.0f;
        CubeColl->GetComponent<OBBCollider>()->height = 0.5f;
        CubeColl->GetComponent<OBBCollider>()->length = 2.0f;
        CubeColl->GetComponent<OBBCollider>()->trigger = false;
        */
        
        CubeColl->AddComponent(collider, Cube, 0.1f, &multiShader, &cubeMesh);
        CubeColl->GetComponent<CubeCollider>()->width = 20.0f;
        CubeColl->GetComponent<CubeCollider>()->height = 0.5f;
        CubeColl->GetComponent<CubeCollider>()->length = 4.0f;
        CubeColl->GetComponent<CubeCollider>()->trigger = false;


        Object* track = new Object(multiShader);
        track->transform.pos = glm::vec3(0, -0.5f, 0);
        track->transform.scale = glm::vec3(2.25f, 2.25f, 2.25f);
        track->AddComponent(mesh, &trackMesh); 
        
        /*
        Object* Tracker = new Object(multiShader);
        Tracker->name = "Tracker";
        //Tracker->transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
        Tracker->transform.pos.x = CubeColl->GetComponent<CubeCollider>()->pos.x + CubeColl->GetComponent<CubeCollider>()->width;
        Tracker->transform.pos.y = CubeColl->GetComponent<CubeCollider>()->pos.y;
        Tracker->transform.pos.z = CubeColl->GetComponent<CubeCollider>()->pos.z;

        Tracker->transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);
        Tracker->AddComponent(mesh, &sphereMesh); 
        */

        Object* Rain = new Object(multiShader);
        Rain->name = "Rain";
        Rain->transform.scale = glm::vec3(1.0f);
        Rain->transform.pos = car->rigidbody->heading + car->rigidbody->pos;
        Rain->objTexture = &waterText;
        Rain->AddComponent(particleSystem, glm::vec3( 0, 50, 0 ), 600.0f, 10000.0f, 5.0f, 5.0f, &planeMesh, true);
        Rain->GetComponent<ParticleSystem>(particleSystem)->speed = glm::vec3(0, -1.0f, 0);
        Rain->GetComponent<ParticleSystem>()->looping = true;

        Object* boostPad = new Object(multiShader);
        boostPad->name = "JUMPPAD";
        boostPad->transform.scale = glm::vec3(1.0f);
        boostPad->transform.pos = {-58.0f, 1.0f, 63.0f};
        //boostPad->transform.pos = {0, 0.5f, 5.0f};
        boostPad->objTexture = &boostTexture;

        boostPad->AddComponent(particle, 600.0f);
        boostPad->AddComponent(mesh, &boostMesh);
        boostPad->AddComponent(collider, Sphere, 1.0f, &multiShader, &colliderMesh);

        
        boostPad->AddScript(new SpeedPad(boostPad->GetComponent<Collider>(collider)));
        boostPad->GetScript<SpeedPad>()->speedBoost = 32.5;
        boostPad->GetScript<SpeedPad>()->direction = up;
        

        boostPad->GetComponent<SphereCollider>()->trigger = true;
        boostPad->GetComponent<SphereCollider>()->render = false;

        boostPad->GetComponent<Particle>()->bounce = 1.0f;
        boostPad->GetComponent<PhysicsSystem>()->gravity = false;
        boostPad->GetComponent<Particle>()->movable = false;

        Object* boostPad2 = new Object(multiShader);
        boostPad2->name = "BOOSTPAD";
        boostPad2->transform.scale = glm::vec3(1.0f);
        boostPad2->transform.rotation = glm::vec3(0.0f, 0.0f, -90.0f);
        boostPad2->transform.pos = {-199.0f, 0.0f, 286.0f};
        //boostP2ad->transform.pos = {0, 0.5f, 5.0f};
        boostPad2->objTexture = &boostTexture;
                
        boostPad2->AddComponent(particle, 20.0f);
        boostPad2->AddComponent(mesh, &boostMesh);
        boostPad2->AddComponent(collider, Sphere, 1.0f, &multiShader, &colliderMesh);
        boostPad2->AddScript(new SpeedPad(boostPad2->GetComponent<Collider>()));
                
        boostPad2->GetScript<SpeedPad>()->speedBoost = 50.0f;
        boostPad2->GetScript<SpeedPad>()->direction = forward;
                
        boostPad2->GetComponent<SphereCollider>()->trigger = true;
        boostPad2->GetComponent<SphereCollider>()->render = false;
                
        boostPad2->GetComponent<Particle>()->bounce = 1.0f;
        boostPad2->GetComponent<PhysicsSystem>()->gravity = false;
        boostPad2->GetComponent<Particle>()->movable = false;
        
        #pragma region Obstacles

            Object* GrassTerrain = new Object(multiShader);
            GrassTerrain->name = "Dirt";
            GrassTerrain->transform.scale = glm::vec3(5.0f, 0.25f, 10.0f);
            GrassTerrain->transform.pos = {-9.5f, -0.15f, 45.0f};
            GrassTerrain->objTexture = &DirtTexture;
            GrassTerrain->AddComponent(particle, 50.0f);
            GrassTerrain->AddComponent(mesh, &cubeMesh);
            GrassTerrain->AddComponent(collider, Cube, 1.0f, &multiShader, &cubeMesh);
        
            GrassTerrain->GetComponent<Particle>()->gravity = false;
            GrassTerrain->GetComponent<Particle>()->movable = false;
            GrassTerrain->GetComponent<Particle>()->BaseDrag = 0.8f;
            GrassTerrain->GetComponent<CubeCollider>()->width = 5.0f;
            GrassTerrain->GetComponent<CubeCollider>()->height = 0.25f;
            GrassTerrain->GetComponent<CubeCollider>()->length = 10.0f;
            GrassTerrain->GetComponent<CubeCollider>()->trigger = true;
            
            Object* waterTerrain = new Object(multiShader);
            waterTerrain->name = "Dirt";
            waterTerrain->transform.scale = glm::vec3(5.0f, 0.25f, 1.0f);
            waterTerrain->transform.pos = { -115.0f, -0.15f, 65.0f };
            waterTerrain->objTexture = &waterText;
            waterTerrain->AddComponent(particle, 50.0f);
            waterTerrain->AddComponent(mesh, &cubeMesh);
            waterTerrain->AddComponent(collider, Cube, 1.0f, &multiShader, &cubeMesh);

            waterTerrain->GetComponent<Particle>()->gravity = false;
            waterTerrain->GetComponent<Particle>()->movable = false;
            waterTerrain->GetComponent<Particle>()->BaseDrag = -0.5f;
            //waterTerrain->GetComponent<CubeCollider>()->width = 5.0f;
            //waterTerrain->GetComponent<CubeCollider>()->height = 0.25f;
            //waterTerrain->GetComponent<CubeCollider>()->length = 10.0f;
            waterTerrain->GetComponent<CubeCollider>()->trigger = true;



            Object* obstacle = new Object(multiShader);
            obstacle->name = "barrier";
            obstacle->transform.pos = glm::vec3{ 1.0f, 0.5f, 10.0f };
            obstacle->transform.rotation = glm::vec3(0, 0, 0.0f);
            obstacle->transform.scale = glm::vec3(0.51f, 1.0f, 0.51f);

            obstacle->AddComponent(mesh, &cubeMesh);

            obstacle->AddComponent(rigidbody, 60.0f);
            obstacle->GetComponent<Rigidbody>()->gravity = false;
            obstacle->GetComponent<Rigidbody>()->movable = false;
            obstacle->GetComponent<Rigidbody>()->bounce = 1.0f;

            obstacle->AddComponent(collider, Cube, 0.1f, &multiShader, &cubeMesh);
            obstacle->GetComponent<CubeCollider>()->width = 0.51f;
            obstacle->GetComponent<CubeCollider>()->height = 1.0f;
            obstacle->GetComponent<CubeCollider>()->length = 0.51f;
            obstacle->GetComponent<CubeCollider>()->trigger = false;

            Object* OBBobstacle = new Object(multiShader);
            OBBobstacle->name = "OBBbarrier";
            OBBobstacle->transform.pos = glm::vec3{ -5.0f, 0.5f, 10.0f };
            OBBobstacle->transform.rotation = glm::vec3(0, 0, 0.0f);
            OBBobstacle->transform.scale = glm::vec3(0.51f, 1.0f, 0.51f);

            OBBobstacle->AddComponent(mesh, &cubeMesh);

            OBBobstacle->AddComponent(rigidbody, 60.0f);
            OBBobstacle->GetComponent<Rigidbody>()->gravity = false;
            OBBobstacle->GetComponent<Rigidbody>()->movable = false;
            OBBobstacle->GetComponent<Rigidbody>()->bounce = 1.0f;

            OBBobstacle->AddComponent(collider, OBB, 0.1f, &multiShader, &cubeMesh);
            OBBobstacle->GetComponent<OBBCollider>()->width = 0.51f;
            OBBobstacle->GetComponent<OBBCollider>()->height = 1.0f;
            OBBobstacle->GetComponent<OBBCollider>()->length = 0.51f;
            OBBobstacle->GetComponent<OBBCollider>()->trigger = false;

            Object* obstacle2 = new Object(multiShader);
            obstacle2->name = "barrier2";
            obstacle2->transform.pos = glm::vec3{ -20.0f, 0.5f, 66.5f };
            obstacle2->transform.rotation = glm::vec3(0, 0, 0.0f);
            obstacle2->transform.scale = glm::vec3(20.0f, 1.0f, 0.50f);
                
            obstacle2->AddComponent(mesh, &cubeMesh);
                
            obstacle2->AddComponent(rigidbody, 80.0f);
            obstacle2->GetComponent<Rigidbody>()->gravity = false;
            obstacle2->GetComponent<Rigidbody>()->movable = false;
            obstacle2->GetComponent<Rigidbody>()->bounce = 1.0f;
                
            obstacle2->AddComponent(collider, Cube, 0.1f, &multiShader, &cubeMesh);
            obstacle2->GetComponent<CubeCollider>()->width = 20.0f;
            obstacle2->GetComponent<CubeCollider>()->height = 1.0f;
            obstacle2->GetComponent<CubeCollider>()->length = 0.50f;
            obstacle2->GetComponent<CubeCollider>()->trigger = false;

            Object* obstacle3 = new Object(multiShader);
            obstacle3->name = "barrier2";
            obstacle3->transform.pos = glm::vec3{ -247.0f, 0.5f, 288.5f };
            obstacle3->transform.rotation = glm::vec3(0, 0, 0.0f);
            obstacle3->transform.scale = glm::vec3(20.0f, 1.0f, 0.50f);
                
            obstacle3->AddComponent(mesh, &cubeMesh);
                
            obstacle3->AddComponent(rigidbody, 80.0f);
            obstacle3->GetComponent<Rigidbody>()->gravity = false;
            obstacle3->GetComponent<Rigidbody>()->movable = false;
            obstacle3->GetComponent<Rigidbody>()->bounce = 1.0f;
                
            obstacle3->AddComponent(collider, Cube, 0.1f, &multiShader, &cubeMesh);
            //obstacle3->GetComponent<CubeCollider>()->width = 20.0f;
            //obstacle3->GetComponent<CubeCollider>()->height = 1.0f;
            //obstacle3->GetComponent<CubeCollider>()->length = 0.50f;
            obstacle3->GetComponent<CubeCollider>()->trigger = false;

            Object* carObstacle = new Object(multiShader);
            carObstacle->transform.pos = glm::vec3{ -180.0f, 1.0f, 286.0f };
            carObstacle->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            carObstacle->transform.scale = { 0.5, 0.5, 0.5 };
            carObstacle->objTexture = &collTexture;
        
            carObstacle->AddComponent(mesh, &carMesh);

            carObstacle->AddComponent(rigidbody, 20.0f);
            carObstacle->GetComponent<Rigidbody>()->bounce = 0.75f;
            carObstacle->GetComponent<Rigidbody>()->gravity = true;
            carObstacle->GetComponent<Rigidbody>()->movable = true;

            carObstacle->AddComponent(collider, Cube, 0.1f, &multiShader, &cubeMesh);
            carObstacle->GetComponent<CubeCollider>()->originalPos.y = 0.5f;
            carObstacle->GetComponent<CubeCollider>()->width = 0.5f;
            carObstacle->GetComponent<CubeCollider>()->height = 0.25f;
            carObstacle->GetComponent<CubeCollider>()->length = 0.5f;

        #pragma endregion
        
        //If an object is created it is now sent to a openly accessible list in WorldManager.h called GameObjects
        //this is is sent in its constructor
        
        //list of objects
        /*
        std::vector<Object*> objects;
        objects.push_back(car);
        objects.push_back(track);
        objects.push_back(CubeColl);
        objects.push_back(GroundCollider);
        objects.push_back(boostPad);
        objects.push_back(boostPad2);
        objects.push_back(Rain);
        //objects.push_back(Tracker);
        objects.push_back(GrassTerrain);
        objects.push_back(obstacle);
        objects.push_back(carObstacle);
        */

        //collider grid
        //this is my spatial partitioning system for my broad phase collision detection
        Grid* collGrid = new Grid();
        Unit* unit;

        for (Object* obj : GameObjects)
        {
            if (obj->GetComponent<Collider>() != NULL && obj != GroundCollider)
            {
                new Unit(collGrid, obj, obj->transform.pos);
            }
        }

        //building the shapes that will be drawn using the vertex data loded from the OBJ;
        ObjShape skyBox = cubeMesh;

        //hard coded with indices
        Shape SplashScreen(squareData, squareFaces);
        Shape Water(squareData, squareFaces);

        //LightObj lightShape(cubeMesh);
        
    #pragma endregion


    #pragma endregion

    #pragma region Game Loop

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //detect inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, KeyInput); 
    glfwSetCursorPosCallback(window, MouseInput);

    int frameCount = 0;
    double oldTime = 0;
    double deltaTime;
    double previousTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        //deltaTime = currentTime - oldTime;

        oldTime = currentTime;
        currentTime = glfwGetTime(); //get the "new" current time
        deltaTime = currentTime - oldTime; //find out the difference between old and new time

        frameCount++;

        if ((currentTime - previousTime) >= 1.0f)
        {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            previousTime = currentTime;
        }


        #pragma region setting up matrices and shader cameras
        
        glm::mat4 modelMatrix(
            0.25, 0, 0, 0.25,
            0, 0.25, 0, 0.5,
            0, 0, 0.25, 0,
            0, 0, 0, 1);

        glm::mat4 projMatrix(
            (float)1 / (halfFov * aspectRatio), 0, 0, 0,
            0, (float)1 / halfFov, 0, 0,
            0, 0, (nearPlane + farPlane) / (nearPlane - farPlane), -1,
            0, 0, (2.0f * nearPlane * farPlane) / (nearPlane - farPlane), 0);

        glm::mat4 viewMatrix = camera->GetViewMatrix();


        //build camera using shader
        multiShader.buildCam(modelMatrix, viewMatrix, projMatrix);
        unlit.buildCam(modelMatrix, viewMatrix, projMatrix);
        cubeMapShader.buildCam(modelMatrix, viewMatrix, projMatrix);
        colliderShader.buildCam(modelMatrix, viewMatrix, projMatrix);

        #pragma endregion

        //clear the framebuffer/window to a background colour
        const float backgroundColour[] = { 0.175, 0.175, 0.175, 0.0f };
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, backgroundColour);

        glfwPollEvents();


        #pragma region Setting Up Objects

        glm::vec3* pos = new glm::vec3();
        glm::vec3* scale = new glm::vec3();

        #pragma region Lighting Objects

        //these now control where the lights are
        glm::vec3 sunVec = { 0, -2, 5 };
        glm::vec3 lightVec = { 0.25f, 0.5f, -0.25f };

        glm::vec3 spotLightPos = { 0, 2.0f, -3 };
        glm::vec3 spotlightDir = { 0, -1, -5};

        glm::vec3 objColour = { 1.0f, 1.0f, 1.0f };
        glm::vec3 lightCol = { 0.25f, 0.25f, 0.25f };


        *pos = glm::vec3(0, 2, -2);
        *scale = glm::vec3(1, 0.5f, 1);

        /*
        objColour = { 0.6f, 0, 0 };
        unlit.assignVec3("objectColour", lightShape.lightcolour);
        unlit.changeTransform({ 0, 2, 5 }, *scale, glm::vec3(60, 0, 0));
        lightShape.Draw();*/

        #pragma endregion

        multiShader.use();

        #pragma region defining lights

        //assigning the different light points
        float* lightStrength = new float(0.1f); //higher num = lower light
        float* falloffStrength = new float(0.5f); //higher num = smaller the reach of the light

        glm::vec3*  ambient = new glm::vec3(0.5 , 0.5 , 0.5 ); //lower value = less light reflected = darker shape
        glm::vec3*  diffuse = new glm::vec3(0.5 , 0.5 , 0.5 );
        glm::vec3* specular = new glm::vec3(0.75, 0.75, 0.75);
        float* shininess = new float(32);
        
        //assiging the default material values for this shader.
        multiShader.assignVec3("material.ambient",  *ambient  );
        multiShader.assignVec3("material.diffuse",  *diffuse  );
        multiShader.assignVec3("material.specular", *specular );
        multiShader.setFloat("material.shininess",*shininess);
        
        // directional light
        multiShader.assignVec3("directional.lightColour", 1.0f, 1.0f, 1.0f);
        multiShader.assignVec3("directional.direction", sunVec);
        multiShader.assignVec3("directional.ambient", 0.5f, 0.5f, 0.5f);
        multiShader.assignVec3("directional.diffuse", 0.25f, 0.25f, 0.25f);
        multiShader.assignVec3("directional.specular", 0.25, 0.25, 0.25);


        delete lightStrength;
        delete falloffStrength;
        delete ambient;
        delete specular;
        delete diffuse;

        #pragma endregion

        #pragma endregion



        #pragma region Updating Objects

        texture2.Use();

        //if the splashcreen should be shown, show it and skip the rest of the loop
        if (splashScreen)
        {
            multiShader.use();
            SplashText.Use();
            multiShader.assignVec3("objectColour", { 1,1,1 });
            multiShader.assignVec3("lightCol", { 1,1,1 });
            multiShader.assignVec3("viewPos", camera->position);
            multiShader.changeTransform( { camera->position.x, camera->position.y, camera->position.z - 2.0 }, glm::vec3(1.5, 1.5, 1.5), glm::vec3(-90, 0, 0));
            SplashScreen.Draw();
            glfwSwapBuffers(window);
            continue;
        }

        multiShader.use();
        //placing models
        objColour = { 1.0f, 1.0f, 1.0f };
        multiShader.assignVec3("objectColour", objColour);
        multiShader.assignVec3("lightCol", lightCol);
        multiShader.assignVec3("viewPos", camera->position);


        //Physics Update
        std::vector<Object*> collList;
        //for (Object* obj : GameObjects)
        for (int i = 0; i < GameObjects.size(); i++)
        {
            Object* obj = GameObjects[i];
            obj->UpdateScripts(deltaTime);
            obj->Update(deltaTime);

            if (obj->GetComponent<Collider>(collider) != NULL)
            {
                if (obj->GetComponent<PhysicsSystem>())
                {
                    obj->GetComponent<PhysicsSystem>()->drag = obj->GetComponent<PhysicsSystem>()->BaseDrag;
                    for (Object* collided : obj->GetComponent<Collider>()->collidedObjs)
                    {
                        if (collided->GetComponent<PhysicsSystem>())
                        {
                            obj->GetComponent<PhysicsSystem>()->drag -= collided->GetComponent<PhysicsSystem>()->BaseDrag;
                        }
                    }
                }
                obj->GetComponent<Collider>()->collidedObjs.clear();
                collList.push_back(obj);
            }
        }

        //Check collisions
        int i = 0;

        collGrid->handleClash();

        for(Object* obj : collList)
        {
            if (obj->tag != Ground)
            {
                //GroundCollider->GetComponent<PlaneCollider>()->CheckForCollision(obj);
                if (obj->GetComponent<Collider>()->CheckForCollision(GroundCollider)) 
                {
                    collGrid->handleResponse(obj, GroundCollider);
                    //collGrid->handleResponse(GroundCollider, obj);
                }
            }
            //
            // ---OPTIMISATION---
            // old way of checkign all the colliders, used to do 2 for loops that searched every item
            // now I use broad and narrow phase collision detection thanks to my spatial partioning
            //
            /*
            Collider* curr = obj->GetComponent<Collider>(collider);
            curr->collidedObjs.clear();
            //std::cout << curr->owner->name << ": " << glm::to_string(curr->owner->transform.pos) << std::endl;
            //collList.erase(collList.begin() + i);
            
                for (int i2 = 0; i2 < collList.size(); i2++)
                {
                    if (i2 != i)
                    {
                        //curr->CheckForCollision(collList[i2]);
                    }
                    else continue;
                }
            //curr->CheckForCollision(collList);
            //collList.insert(collList.begin() + i, curr->owner);
            i++;*/
        }

        collList.clear();

        //Draw Update
        for (int i = 0; i < GameObjects.size(); i++)
        {
            GameObjects[i]->Render(deltaTime);
        }

        camera->Follow(CurrentPlayer);

        #pragma region Skybox
        * pos = glm::vec3(0, 5, 0);
        *scale = glm::vec3(120, 120, 120);
        glDepthMask(GL_FALSE);
        CubemapText.Use();
        //cubeMapShader.use();
        cubeMapShader.setInt("skybox", 0);
        cubeMapShader.changeTransform(camera->position, *scale);
        skyBox.Draw();
        glDepthMask(GL_TRUE);
        #pragma endregion



        //finished drawing, clean up pointers
        delete pos;
        delete scale;

        #pragma endregion
        
        #pragma endregion

        //Redraw the contents of the window and present it to the user ("paints the screen")
        glfwSwapBuffers(window);

    }

    #pragma endregion

    //CLEANUP

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
