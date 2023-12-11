#include "std.h"
#include "transform.h"
//--- 메인 함수
//--- 함수 선언 추가하기

GLuint window_w = 1000;
GLuint window_h = 1000;

unsigned int lightPosLocation;
unsigned int lightColorLocation;
unsigned int viewLocation;

glm::vec3 lightPos(7, 10, 10);
glm::vec3 lightColor(0.8, 0.8, 0.8);
glm::vec3 cameraPos(-0.25, +1.0, +1); //--- 카메라 위치

float light_x = 7;
float light_y = 10;
float light_z = 10;
float zcamera;
int road_count = 500;
float road_x_move[500];
float road_y_move[500];
float road_z_move[500];

struct Vertices {
	glm::vec3 pos;
	glm::vec3 nor;
};
std::vector<Vertices> m_vertices;

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

bool left_button;
bool spacebar;
bool jump;
bool floortouch = false;
float zspherespeed = 0.01f;
float xspherespeed = 0.01f;
float yspherespeed = 0.f;
float gravity = 0.0098f;
float power = 0.2f;
float fTime = 1.f;
float fTime2 = 1.f;
glm::vec3 Tsphere;

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
	for (int i = 0; i < road_count; i++) {
		if (road_y_move[i] < -0.5 && isPointInRect(road_x_move[i], road_z_move[i],
			cameraPos.x - 0.6, cameraPos.z - 0.6, cameraPos.x + 0.6, cameraPos.z + 0.6))
		{
			road_y_move[i] += 0.002;
		}
	}

	if (!spacebar) {		// 스페가 안눌렸었다면 true
		Tsphere = glm::vec3(xspherespeed, yspherespeed -= gravity, zspherespeed -= 0.0001f);
	}
	else {					// 스페가 눌렸었다면 false;
		Tsphere = glm::vec3(xspherespeed += 0.0001f, yspherespeed -= gravity, zspherespeed);
	}
	/*for (int i = 0; i < road_count; ++i) {
		if (yspherespeed < )
			yspherespeed = road_y_move[i] - 1.f;
	}*/

	std::cout << "y구 - " << yspherespeed << " 맵y - " << road_y_move[0] << '\n';

	//box_scale = glm::scale(box_scale, glm::vec3(0.2, 0.2, 0.2));
	//for (int i = 0; i < 500; i++) {
	//	map_move[i] = glm::translate(map_move[i], glm::vec3(road_x_move[i], road_y_move[i], road_z_move[i]));

	//	box[i] = map_move[i] * box_scale;
	//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(box[i]));

	if (jump) {		// 증가값이 일정값 넘으면 역방향
		Tsphere = glm::vec3(xspherespeed, yspherespeed += (power * fTime - (gravity * (fTime * fTime))) * 0.5f, zspherespeed);
		fTime += 0.04;
		std::cout << yspherespeed << '\t' << (power * fTime - (gravity * (fTime * fTime))) << '\n';
		for (int i = 0; i < road_count; ++i) {
			if (yspherespeed < road_y_move[i] - 1.f) {
				yspherespeed = road_y_move[i] - 1.f;
				jump = false;
				fTime = 0.f;
			}
		}
	}

	glutPostRedisplay();
	//if(spacebar)
		//glutTimerFunc(100, timerfunc, 1);

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
	switch (key) {
	case GLUT_KEY_UP:
	{
		cameraPos.z -= 0.1;
	}
	break;
	case GLUT_KEY_DOWN:
		cameraPos.z += 0.1;
		break;
	case GLUT_KEY_LEFT:
		cameraPos.x -= 0.1;
		printf("%f %f %f %f %f %f %f %f", cameraPos.x - 2.0, cameraPos.z - 2.0, cameraPos.x + 2.0, cameraPos.z + 2.0,
			road_x_move[0] - 2.0, road_z_move[0] - 2.0, road_x_move[0] + 2.0, road_z_move[0] + 2.0);
		break;
	case GLUT_KEY_RIGHT:
		cameraPos.x += 0.1;

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
	{
		if (!spacebar) {		// 스페가 안눌렸었다면 true
			spacebar = true;
		}
		else {					// 스페가 눌렸었다면 false;
			spacebar = false;
		}
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

	ReadObj("sphere.obj");
	//ReadObj("nescafe_mug.obj");

	glEnable(GL_DEPTH_TEST);  // 깊이 테스트 활성화
	glutMainLoop();


}

GLvoid drawScene()
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(vao);

	glm::vec3 cameraDirection = glm::vec3(cameraPos.x + 0.25, cameraPos.y - 1.5, cameraPos.z - 1); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//--- 사용할 VAO 불러오기
	glm::mat4 Tx = glm::mat4(1.0f); //--- 이동 행렬 선언
	glm::mat4 Rz = glm::mat4(1.0f); //--- 회전 행렬 선언
	glm::mat4 TR = glm::mat4(1.0f);
	glm::mat4 Sc = glm::mat4(1.0f);
	glm::mat4 box[500] = { glm::mat4(1.0f) };

	glm::mat4 box_scale = glm::mat4(1.0f);
	glm::mat4 map_move[500] = { glm::mat4(1.0f) };
	for (int i = 0; i < 500; i++) {
		map_move[i] = glm::mat4(1.0f);
		box[i] = glm::mat4(1.0f);
	}
	GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	GLuint projLoc = glGetUniformLocation(shaderProgramID, "projection");

	glm::vec3 newColor(0.0f, 0.0f, 0.0f);

	glm::mat4 mTransform = glm::mat4(1.0f);
	Tx = glm::translate(glm::mat4(1.0f), Tsphere);
	Rz = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Sc = glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));

	mTransform = Sc * Tx;
	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mTransform));

	glm::mat4 vTransform = glm::mat4(1.0f);
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	vTransform = vTransform; //카메라위치 돌리기 앞에두면 카메라 자전, 뒤에 두면 카메라 공전
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);
	
	perspective(shaderProgramID, 0.0f);

	//lightPos = glm::vec3(lightPos.x, lightPos.y, lightPos.z);
	//float light = 1 - abs((ballPos.y-5)/ 140);
	//lightColor = glm::vec3(lightColor.x, lightColor.y, lightColor.z);

	 {   //조명 위치
		glm::vec4 lightPosInModelSpace = glm::vec4(light_x, light_y, light_z, 1.0f);
		lightPosInModelSpace = lightPosInModelSpace;
		// 셰이더에 변환된 조명 위치 전달
		GLuint lightPosLoc = glGetUniformLocation(shaderProgramID, "lightPos");
		glUniform3f(lightPosLoc, lightPosInModelSpace.x, lightPosInModelSpace.y, lightPosInModelSpace.z);

	}

	//box_scale = glm::scale(box_scale, glm::vec3(0.2, 0.2, 0.2));

	newColor.r = 1.0; newColor.g = 1.0; newColor.b = 1.0;
	//box = box * box_scale;
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(box));

	timerfunc(10);

	UpdateBuffer();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	glBindVertexArray(map_vao);
	box_scale = glm::scale(box_scale, glm::vec3(0.2, 0.2, 0.2));
	for (int i = 0; i < 500; i++) {
		map_move[i] = glm::translate(map_move[i], glm::vec3(road_x_move[i], road_y_move[i], road_z_move[i]));

		box[i] = map_move[i] * box_scale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(box[i]));
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

	glUseProgram(shaderProgramID);

	unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 0.5, 0.3);
	unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);
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
			if (fscanf(fp, "%f %f %f", &pos.x, &pos.y, &pos.z) != 3) exit(1);
			nor.push_back(pos);
		}
		// vertex texture coordinate
		else if (buff[0] == 'v' && buff[1] == 't' && buff[2] == '\0') {
			glm::vec2 pos;
			if (fscanf(fp, "%f %f", &pos.x, &pos.y) != 2) exit(1);
			tex.push_back(pos);
		}
		else if (buff[0] == 'f' && buff[1] == '\0') {
			Vertices temp;
			int v, t, n;
			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(1);
			temp.pos = vtx[v - 1];
			temp.nor = nor[n - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);

			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(1);
			temp.pos = vtx[v - 1];
			temp.nor = nor[n - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);

			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(1);
			temp.pos = vtx[v - 1];
			temp.nor = nor[n - 1];
			//temp.TexCoordinate = tex[t - 1];
			m_vertices.push_back(temp);

			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(1);
			temp.pos = vtx[v - 1];
			temp.nor = nor[n - 1];
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

