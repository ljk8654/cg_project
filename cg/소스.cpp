#include "std.h"
#include "transform.h"
#include "Sound.h"

//--- 메인 함수
//--- 함수 선언 추가하기

GLuint window_w = 1000;
GLuint window_h = 900;

unsigned int lightPosLocation;
unsigned int lightColorLocation;
unsigned int viewLocation;

glm::vec3 lightPos(0, 100, 0);
glm::vec3 lightColor(0.8, 0.8, 0.8);
glm::vec3 cameraPos(-0.25, +1.0, +1); //--- 카메라 위치

int road_count = 300;
float road_x_move[300];
float road_y_move[300];
float road_z_move[300];
GLfloat APS = 0.25;
GLfloat yRotate = 0;
float yRotateDirection = 1.0f;
GLfloat currentTime = 0.0f;
GLfloat frameTime = 0.0f;


struct Vertices {
	glm::vec3 pos;
	glm::vec3 nor;
};
std::vector<Vertices> m_vertices;

struct Snow {
	float x, y, z;
	float fast;
};

Snow snow[100];

void make_snow(int i) {

		snow[i].x = cameraPos.x + float(rand() % 10) / 10 - 0.5;
		snow[i].z = cameraPos.z + float(rand() % 10) / 10 - 0.5;
		snow[i].y = 1.0;
		snow[i].fast = float(rand() % 20) / 10000 + 0.0001;
}

float vertices[] = { //--- 버텍스 속성: 좌표값(FragPos), 노말값 (Normal)
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

GLuint vao, vbo[2], map_vao, map_vbo[2];
GLuint TriPosVbo[15], TriColorVbo[15];

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID;//--- 셰이더 프로그램

void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void InitBuffer();
void UpdateBuffer();
void make_map();
char* filetobuf(const char*);
void ReadObj(const char* fileName);
void ReadObj2(const char* fileName);
void timerfunc(int value);

bool spacebar;
bool jump;
float zspherespeed = 0.01f;
float xspherespeed = 0.01f;
float yspherespeed = -0.3f;
float gravity = 0.001f;
float power = 0.017f;
float fTime = 0.1f;
glm::vec3 Tsphere;

SoundManager* soundManager = nullptr;

bool isRectCollision(float rect1_left, float rect1_bottom, float rect1_right, float rect1_top,
	float rect2_left, float rect2_bottom, float rect2_right, float rect2_top) {
	// 충돌 검사
	if ((rect1_left < rect2_right) && (rect1_top < rect2_bottom) && (rect1_right > rect2_left) &&
		(rect1_bottom > rect2_top)) {
		printf("222");

		return true;  // 충돌이 있음

	}
	else {
		return false;  // 충돌이 없음
	}
}
bool isPointInRect(float x, float y, float rect_left, float rect_bottom, float rect_right, float rect_top) {
	return (x >= rect_left && x <= rect_right && y >= rect_bottom && y <= rect_top);
}

void timerfunc(int value) {
	if (currentTime == 0.0f) {
		currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;  // 초 단위로 변환
	}
	GLfloat newTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;  // 초 단위로 변환
	frameTime = newTime - currentTime;
	currentTime = newTime;

	//눈 내리기
	for (int i = 0; i < 100; i++) {
		snow[i].y -= snow[i].fast;
		if (snow[i].y < 0) make_snow(i);
	}

	//길 움직이기
	for (int i = 0; i < road_count; i++) {
		if (road_y_move[i] < -0.5 && isPointInRect(road_x_move[i], road_z_move[i],
			cameraPos.x - 1.5, cameraPos.z - 1.5, cameraPos.x + 1.5, cameraPos.z + 1.5))
		{
			road_y_move[i] += 0.01;
		}
	}

	if (!spacebar) {	// 우측 방향 z 보는 방향 기준
		Tsphere = glm::vec3(xspherespeed, yspherespeed -= gravity, zspherespeed -= 0.005f);
	}
	else {				// 좌측 방향 x 보는 방향 기준
		Tsphere = glm::vec3(xspherespeed += 0.005f, yspherespeed -= gravity, zspherespeed);
	}

	for (int i = 0; i < road_count; ++i) {
		if (yspherespeed < road_y_move[i] + 0.2f) {
			if (!(isPointInRect(xspherespeed, zspherespeed, road_x_move[i] - 0.5, road_z_move[i] - 0.5, road_x_move[i] + 0.5, road_z_move[i] + 0.5))) {
				yspherespeed -= 0.005f;
				break;
			}
			else
				yspherespeed = road_y_move[i] + 0.2f;

		}
	}

	if (jump) {		// 증가값이 일정값 넘으면 역방향
		Tsphere = glm::vec3(xspherespeed, yspherespeed += (power * fTime - (gravity * (fTime * fTime))) * 0.5f, zspherespeed);
		power -= 0.00015f;
		if ((power * fTime) < (gravity * fTime * fTime) * 0.5f)
			fTime += 0.03f;
		else
			fTime += 0.1f;

		for (int i = 0; i < road_count; ++i) {
			if (yspherespeed < road_y_move[i] + 0.2f) {
				if (!(isPointInRect(xspherespeed, zspherespeed, road_x_move[i] - 0.5, road_z_move[i] - 0.5, road_x_move[i] + 0.5, road_z_move[i] + 0.5))) {
					std::cout << "떨어져야함" << '\n';
					break;
				}
				else {
					yspherespeed = road_y_move[i] + 0.2f;
					jump = false;
					fTime = 0.f;
					power = 0.017f;
				}
			}
		}
	}
	// 떨어지는 거 아직 미구현

	yRotate += APS * yRotateDirection * frameTime;

	if (yRotate > 10) {
		yRotateDirection = -1.0f;  // 역방향으로 회전
		yRotate = 10.0f;  // 회전 각도 초기화

	}
	else if (yRotate < -10) {
		yRotateDirection = 1.0f;  // 정방향으로 회전
		yRotate = -10.0f;  // 회전 각도 초기화
	}

	glutPostRedisplay();
	glutTimerFunc(1, timerfunc, 0);

}

void Motion(int x, int y) {

	float dot_x = ((float)x - 400) / 400.0;
	float dot_y = (300.0 - (float)y) / 300.0;

	glutPostRedisplay();

}


void Mouse(int button, int state, int x, int y) {
	float dot_x = ((float)x - 400) / 400.0;
	float dot_y = (300.0 - (float)y) / 300.0;

	glutPostRedisplay();


}
void specialKeyCallback(int key, int x, int y) {
	float moveSpeed = 0.1;  // 필요에 따라 이동 속도를 조절하세요
	float angleRad = glm::radians(yRotate);  // 각도를 라디안으로 변환

	switch (key) {
	case GLUT_KEY_UP:
		cameraPos.x -= moveSpeed * sin(angleRad);
		cameraPos.z -= moveSpeed * cos(angleRad);
		break;
	case GLUT_KEY_DOWN:
		cameraPos.x += moveSpeed * sin(angleRad);
		cameraPos.z += moveSpeed * cos(angleRad);
		break;
	case GLUT_KEY_LEFT:
		cameraPos.x -= moveSpeed * cos(angleRad);
		cameraPos.z += moveSpeed * sin(angleRad);
		break;
	case GLUT_KEY_RIGHT:
		cameraPos.x += moveSpeed * cos(angleRad);
		cameraPos.z -= moveSpeed * sin(angleRad);
		break;
	case GLUT_KEY_CTRL_L:
		if (!jump) {
			jump = true;
		}
		break;
	case GLUT_KEY_PAGE_DOWN:
		cameraPos.y -= 0.01;
		break;
	case GLUT_KEY_PAGE_UP:
		cameraPos.y += 0.01;
		break;
	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key)
	{
	case 32:
		if (!spacebar) {		// 스페가 안눌렸었다면 true
			spacebar = true;
		}
		else {					// 스페가 눌렸었다면 false;
			spacebar = false;
		}
		break;
	}
	glutPostRedisplay();

}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Dancing Lines");
	glewExperimental = GL_TRUE;
	glewInit();
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
	}

	make_shaderProgram();
	InitBuffer();
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(specialKeyCallback);
	glutDisplayFunc(drawScene);

	soundManager = new SoundManager();
	soundManager->PlayBGMSound(BGMSound::Normal, 0.2f, GL_TRUE);
	glutTimerFunc(1, timerfunc, 0);
	ReadObj("sphere.obj");
	//ReadObj2("bed.obj");
	//timerfunc(10);
	glEnable(GL_DEPTH_TEST);  // 깊이 테스트 활성화
	glutMainLoop();
}

GLvoid drawScene()
{
	unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(vao);

	//--- 사용할 VAO 불러오기
	glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
	glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
	glm::mat4 TR = glm::mat4(1.0f);
	glm::mat4 Sc = glm::mat4(1.0f);
	glm::mat4 box[300] = { glm::mat4(1.0f) };
	glm::mat4 box_scale = glm::mat4(1.0f);
	glm::mat4 map_move[300] = { glm::mat4(1.0f) };
	glm::mat4 snow_obj[100] = { glm::mat4(1.0f) };
	glm::mat4 snow_scale = glm::mat4(1.0f);
	glm::mat4 obj_scale = glm::mat4(1.0f);
	glm::mat4 snow_move[100] = { glm::mat4(1.0f) };
	glm::mat4 Oobj = glm::mat4(1.0f);


	for (int i = 0; i < 300; i++) {
		map_move[i] = glm::mat4(1.0f);
		box[i] = glm::mat4(1.0f);
	}
	for (int i = 0; i < 100; i++) {
		snow_move[i] = glm::mat4(1.0f);
		snow_obj[i] = glm::mat4(1.0f);

	}

	GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	GLuint projLoc = glGetUniformLocation(shaderProgramID, "projection");

	glm::vec3 newColor(0.0f, 0.0f, 0.0f);

	glm::mat4 mTransform = glm::mat4(1.0f);
	Tx = glm::translate(glm::mat4(1.0f), Tsphere);
	Sc = glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));

	mTransform = Tx * Sc;
	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mTransform));


	cameraPos.x = xspherespeed- 1.0;
	cameraPos.y = yspherespeed + 1.5;
	cameraPos.z = zspherespeed + 1.0;
	perspective(shaderProgramID, 0.0f);
	CameraThird(shaderProgramID, glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z), glm::vec3(xspherespeed+0.3, yspherespeed+0.3, zspherespeed-0.1), yRotate);
    
	lightPos = glm::vec3(xspherespeed, yspherespeed + 0.3, zspherespeed);
	float light = 1 - abs((yspherespeed - 5) / 140);
	lightColor = glm::vec3(light, light, light);
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(viewLocation, cameraPos.x, cameraPos.y, cameraPos.z);


	//box_scale = glm::scale(box_scale, glm::vec3(0.2, 0.2, 0.2));

	newColor.r = 1.0; newColor.g = 1.0; newColor.b = 1.0;
	//box = box * box_scale;
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(box));


	UpdateBuffer();
	//glBindVertexArray(vao);
	//glUniform3f(objColorLocation, 0.7, 0.7, 0.0);
	//Oobj = glm::translate(Oobj, glm::vec3(0, 0.0, 0));
	//obj_scale = glm::scale(obj_scale, glm::vec3(0.2, 0.2, 0.2));

	//Oobj *= obj_scale;
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Oobj));
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	glBindVertexArray(map_vao);
	box_scale = glm::scale(box_scale, glm::vec3(0.2, 0.2, 0.2));
	for (int i = 0; i < 300; i++) {
		map_move[i] = glm::translate(map_move[i], glm::vec3(road_x_move[i], road_y_move[i], road_z_move[i]));
		box[i] = map_move[i] * box_scale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(box[i]));
		if(i % 100 == 38) glUniform3f(objColorLocation, 0.0, 0.0, 0.0);
		else glUniform3f(objColorLocation, 0.7, 0.7, 0.7);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


	glUniform3f(objColorLocation, 1.0, 1.0, 1.0); // 객체 색 바꿈
	snow_scale = glm::scale(snow_scale, glm::vec3(0.002, 0.002, 0.002));
	for (int i = 0; i < 100; i++) {
		snow_move[i] = glm::translate(snow_move[i], glm::vec3(snow[i].x, snow[i].y, snow[i].z));
		snow_obj[i] = snow_move[i] * snow_scale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(snow_obj[i]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glutSwapBuffers();

}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
//카메라 위치에 따라서 땅이 올라옴 충돌박스?
void InitBuffer()
{
	make_map();
	for(int i=0; i< 100; i++) make_snow(i);
	//ReadObj("mushroom.obj");
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo[0]);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertices), &m_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices), (void*)offsetof(Vertices, nor)); //--- 노말 속성
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &map_vao);
	glGenBuffers(1, &map_vbo[0]);
	glBindVertexArray(map_vao);
	glBindBuffer(GL_ARRAY_BUFFER, map_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(1);

	lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos");
	lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor");
	viewLocation = glGetUniformLocation(shaderProgramID, "viewPos");
}

void UpdateBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);		// Position Color
	if (!m_vertices.empty())
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertices), &m_vertices.front(), GL_STATIC_DRAW);
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}

void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cout << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}

void ReadObj(const char* fileName)
{
	FILE* fp = fopen(fileName, "r");

	if (fp == NULL) {
		std::cerr << "Invalid Object File!!\n";
		exit(100);
	}

	char buff[100];
	int faceNum = 0;
	std::vector<glm::vec3> vtx;
	std::vector<glm::vec3> nor;
	std::vector<glm::vec2> tex;

	while (!feof(fp)) {
		if (fscanf(fp, "%s", buff) == 3) printf("hello it's 3");

		// vertex or element buffer
		if (buff[0] == 'v' && buff[1] == '\0') {
			glm::vec3 pos;
			if (fscanf(fp, "%f %f %f", &pos.x, &pos.y, &pos.z) != 3) exit(1);
			vtx.push_back(pos);
		}
		// vertex normal
		else if (buff[0] == 'v' && buff[1] == 'n' && buff[2] == '\0') {
			glm::vec3 pos;
			if (fscanf(fp, "%f %f %f", &pos.x, &pos.y, &pos.z) != 3) exit(2);
			nor.push_back(pos);
		}
		// vertex texture coordinate
		else if (buff[0] == 'v' && buff[1] == 't' && buff[2] == '\0') {
			glm::vec2 pos;
			if (fscanf(fp, "%f %f", &pos.x, &pos.y) != 2) exit(6);
			tex.push_back(pos);
		}
		else if (buff[0] == 'f' && buff[1] == '\0') {
			Vertices temp;
			int v, t, n;
			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(5);
			temp.pos = vtx[v - 1];
			temp.nor = nor[n - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);

			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(7);
			temp.pos = vtx[v - 1];
			temp.nor = nor[n - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);

			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(8);
			temp.pos = vtx[v - 1];
			temp.nor = nor[n - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);
		}

		memset(buff, NULL, sizeof(buff));
	}
	vtx.clear();
	nor.clear();
	tex.clear();
	fclose(fp);
} 


void ReadObj2(const char* fileName)
{
	FILE* fp = fopen(fileName, "r");

	if (fp == NULL) {
		std::cerr << "Invalid Object File!!\n";
		exit(100);
	}

	char buff[100];
	int faceNum = 0;
	std::vector<glm::vec3> vtx;
	std::vector<glm::vec3> nor;
	std::vector<glm::vec3> tex;

	while (!feof(fp)) {
		if (fscanf(fp, "%s", buff) == 3) printf("hello it's 3");

		// vertex or element buffer
		if (buff[0] == 'v' && buff[1] == '\0') {
			glm::vec3 pos;
			if (fscanf(fp, "%f %f %f", &pos.x, &pos.y, &pos.z) != 3) exit(1);
			vtx.push_back(pos);
		}
		// vertex normal
		else if (buff[0] == 'v' && buff[1] == 'n' && buff[2] == '\0') {
			glm::vec3 pos;
			if (fscanf(fp, "%f %f %f", &pos.x, &pos.y, &pos.z) != 3) exit(2);
			nor.push_back(pos);
		}
		// vertex texture coordinate
		else if (buff[0] == 'v' && buff[1] == 't' && buff[2] == '\0') {
			glm::vec3 pos;
			if (fscanf(fp, "%f %f", &pos.x, &pos.y) != 2) exit(6);
			tex.push_back(pos);
		}
		else if (buff[0] == 'f' && buff[1] == '\0') {
			Vertices temp;
			int v, t, n;
			if (fscanf(fp, "%d/%d", &v, &t) != 2) exit(5);
			temp.pos = vtx[v - 1];
			temp.nor = tex[t - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);

			if (fscanf(fp, "%d/%d", &v, &n) != 2) exit(7);
			temp.pos = vtx[v - 1];
			temp.nor = tex[n - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);

			if (fscanf(fp, "%d/%d", &v, &n) != 2) exit(8);
			temp.pos = vtx[v - 1];
			temp.nor = tex[n - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);
		}

		memset(buff, NULL, sizeof(buff));
	}


	vtx.clear();
	nor.clear();
	tex.clear();

	fclose(fp);
}
void make_map() {
	float road_volume = 0.2;
	int x_inc_count = 0;
	int z_inc_count = 0;
	for (int i = 0; i < road_count; i++) {
		road_x_move[i] = x_inc_count * road_volume;
		road_y_move[i] = -1.0;
		road_z_move[i] = -z_inc_count * road_volume;

		int r = rand() % 2;
		if (r == 0) x_inc_count++;
		else z_inc_count++;
	}
}
//--- out_Color: 버텍스 세이더에서 입력받는 색상 값
//--- FragColor: 출력할 색상의 값으로 프레임 버퍼로 전달 됨.

