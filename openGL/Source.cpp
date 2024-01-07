#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
/*
點(Point) :
    glBegin(GL_POINTS);
    glVertex2f(0.0, 0.0);
    glEnd();

直線(Line) :
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);  
    glVertex2f(1.0, 0.0);  
    glEnd();
三角形(Triangle) :
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 1.0);  
    glVertex2f(-1.0, -1.0); 
    glVertex2f(1.0, -1.0);  
    glEnd();
矩形(Rectangle) :
    glBegin(GL_QUADS);
    glVertex2f(-1.0, 1.0);  // 左上角
    glVertex2f(1.0, 1.0);   // 右上角
    glVertex2f(1.0, -1.0);  // 右下角
    glVertex2f(-1.0, -1.0); // 左下角
    glEnd();
圓形(Circle) :
    float centerX = 0.0f;
    float centerY = 0.0f;
    float radius = 0.5f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);  // 圓心

    for (float angle = 0.0; angle <= 2 * 3.14159; angle += 0.1) {
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
立方體(Cube) :
    glutSolidCube(1.0); 
球體(Sphere) :
    glutSolidSphere(0.5, 20, 20);  
圓錐體(Cone) :
    glutSolidCone(0.5, 1.0, 20, 20); 
圓環(Torus) :
    glutSolidTorus(0.2, 0.5, 20, 20);  
*/
#define PI 3.14159265358979323846

inline float radian(float degree) {
    return (PI / 180) * degree;
}

#pragma region 球體運動
float cameraDistance = 5.0;  // 相機與球體中心的初始距離
float cameraAzimuth = 0.0;  // 方位角
float cameraElevation = 0.0;  // 仰角
float cameraSpeed = 0.005;  // 相機移動速度
int lastX = 0;
int lastY = 0;

float light23radian = 3.5;
float light_Azimuth2 = 45.0;
float light_Elevation2 = 0.0;
float light_Azimuth3 = 135.0;
float light_Elevation3 = 0.0;
float light_speed = 0.15;
bool light_Elevation_turn2 = true;
bool light_Elevation_turn3 = true;
bool light_visible = true;

bool leftButtonPressed = false;
bool rightButtonPressed = false;
#pragma endregion

#pragma region 機器人身體設定
const float head_size = 1.0;
const float body_height = 2.7;
const float arm_thickness = 0.3;
const float leg_thickness = 0.7;
const float half_arm_length = 1.6;
const float half_leg_length = 2.1;
const float body_space_with_head = 0.1;
const float arm_space_with_body = 0.1;
const float bigarm_space_with_smallarm = 0.1;
const float leg_space_with_body = 0.1;
const float bigleg_space_with_smallleg = 0.1;
const float space_between_leg = 0.1;
const float initial_head_y = 2.5;
float head_y = initial_head_y;                                         //機器人y軸位置改這
//下面都是先初始化而已
float body_width = 0.0;
float bofy_depth = 0.0;
float body_y = 0.0;
float leg_y = 0.0;
//走路和跑步的設定
bool walking = false;
bool running = false;
float ups_and_downs_walk_speed = 0.0004;
float ups_and_downs_run_speed = 0.004;
float calf_rorate_right = 0.0;
float calf_rorate_left = 0.0;
const float max_ups_and_downs_walk = 0.08;
const float max_ups_and_downs_run = 0.32;
const float arm_walk_max_angle = 10.0;
const float arm_run_max_angle = 80.0;
const float leg_walk_max_angle = 20.0;
const float leg_run_max_angle = 110.0;
float shoulder = 0.0;
float arm_walk_speed = 0.025;
float arm_run_speed = 0.5;
float leg_angle = 0.0;
float leg_walk_speed = 0.05;
float leg_run_speed_mul = 1;
float leg_run_speed = leg_run_speed_mul * arm_run_speed * leg_run_max_angle / arm_run_max_angle;
bool right_leg_begin_walk = true;
bool right_leg_begin_run = true;
#pragma endregion

#pragma region 材質們
GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat shine[] = { 128.0 };
GLfloat emission[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat specular_little[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat ambient_no[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat diffuse_no[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat specular_no[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shine_no[] = { 0.0 };

GLfloat ambient_eye[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat diffuse_eye[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat specular_eye[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat diffuse_eye_white[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat ambient_mouth[] = { 0.2, 0.0, 0.0, 1.0 };
GLfloat diffuse_mouth[] = { 0.8, 0.0, 0.0, 1.0 };
GLfloat specular_mouth[] = { 0.8, 0.0, 0.0, 1.0 };

GLfloat ambient_bronze[] = { 0.2125, 0.1275, 0.054, 1.0 };
GLfloat diffuse_bronze[] = { 0.714, 0.4284, 0.18144, 1.0 };
GLfloat specular_bronze[] = { 0.393548, 0.271906, 0.166721, 1.0 };
GLfloat shine_bronze[] = { 0.2 * 128 };

GLfloat ambient_silver[] = { 0.1922, 0.19225, 0.19225, 1.0 };
GLfloat diffuse_silver[] = { 0.50754, 0.50754, 0.50754, 1.0 };
GLfloat specular_silver[] = { 0.508273, 0.508273, 0.508273, 1.0 };
GLfloat shine_silver[] = { 51.2 };

GLfloat ambient_gold[] = { 0.44725, 0.4245, 0.0645, 1.0 };
GLfloat diffuse_gold[] = { 0.34615, 0.3143, 0.0903, 1.0 };
GLfloat specular_gold[] = { 0.797357, 0.723991, 0.208006, 1.0 };
GLfloat shine_gold[] = { 51.2 };

GLfloat ambient_emerald[] = { 0.0215, 0.1745, 0.0215, 1.0 };
GLfloat diffuse_emerald[] = { 0.07568, 0.61424, 0.07568, 1.0 };
GLfloat specular_emerald[] = { 0.633, 0.727811, 0.633, 1.0 };
GLfloat shine_emerald[] = { 0.6 * 128 };

GLfloat ambient_chrome[] = { 0.25, 0.25, 0.25, 1.0 };
GLfloat diffuse_chrome[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat specular_chrome[] = { 0.774597, 0.774597, 0.774597, 1.0 };
GLfloat shine_chrome[] = { 0.6 * 128 };

GLfloat ambient_cyan_rubber[] = { 0.0, 0.05, 0.05, 1.0 };
GLfloat diffuse_cyan_rubber[] = { 0.4, 0.5, 0.5, 1.0 };
GLfloat specular_cyan_rubber[] = { 0.04, 0.7, 0.7, 1.0 };
GLfloat shine_cyan_rubber[] = { 0.078125 * 128 };

#pragma endregion

#pragma region 燈光們
GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//上面是預設值
GLfloat d = 3.5;
GLfloat light_position0[] = { 0.0, 0.0, -d, 1.0 };
GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position1[] = { 0.0, 0.0, d, 1.0 };
GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position2[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse2[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position3[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse3[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position4[] = { -d, 0.0, 0.0, 1.0 };
GLfloat light_diffuse4[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position5[] = { d, 0.0, 0.0, 1.0 };
GLfloat light_diffuse5[] = { 1.0, 1.0, 1.0, 1.0 };
#pragma endregion

#pragma region Tehetregon

int tetrahedron_level = 0;
GLfloat v[4][3] = {
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 1.0, 1.0}
};

GLfloat colors[4][3] = {
    {1.0, 1.0, 0.0},  // Yellow
    {0.0, 1.0, 0.0},  // Green
    {1.0, 0.0, 0.0},  // Red
    {0.0, 0.0, 1.0}   // Blue

};

void triangle(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* color) {
    glBegin(GL_TRIANGLES);
    glColor3fv(color);
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
    glEnd();
}

void subdivion_tetrahedron(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, int n) {
    if (n == 0) {

        triangle(a, b, c, colors[0]);
        triangle(a, c, d, colors[1]);
        triangle(a, d, b, colors[2]);
        triangle(b, d, c, colors[3]);
    }
    else {
        GLfloat mid[6][3];
        for (int j = 0; j < 3; j++) mid[0][j] = (a[j] + b[j]) / 2;
        for (int j = 0; j < 3; j++) mid[1][j] = (a[j] + c[j]) / 2;
        for (int j = 0; j < 3; j++) mid[2][j] = (a[j] + d[j]) / 2;
        for (int j = 0; j < 3; j++) mid[3][j] = (b[j] + c[j]) / 2;
        for (int j = 0; j < 3; j++) mid[4][j] = (b[j] + d[j]) / 2;
        for (int j = 0; j < 3; j++) mid[5][j] = (c[j] + d[j]) / 2;

        subdivion_tetrahedron(a, mid[0], mid[1], mid[2], n - 1);
        subdivion_tetrahedron(mid[0], b, mid[3], mid[4], n - 1);
        subdivion_tetrahedron(mid[1], mid[3], c, mid[5], n - 1);
        subdivion_tetrahedron(mid[2], mid[4], mid[5], d, n - 1);
    }
}

void display_tetrahedron() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -5.0, 5.0);
    gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    subdivion_tetrahedron(v[0], v[1], v[2], v[3], tetrahedron_level);
    glFlush();
}


#pragma region menu
void menu_tetrahedron(int value) {
    switch (value) {
    case 0:
        exit(0);
        break;
    case 1:
        tetrahedron_level = 0;
        break;
    case 2:
        tetrahedron_level = 1;
        break;
    case 3:
        tetrahedron_level = 2;
        break;
    case 4:
        tetrahedron_level = 3;
        break;
    }
    glutPostRedisplay();
}

void createMenu_tetrahedron() {
    int subdivion = glutCreateMenu(menu_tetrahedron);
    glutAddMenuEntry("0     ", 1);
    glutAddMenuEntry("1     ", 2);
    glutAddMenuEntry("2     ", 3);
    glutAddMenuEntry("3     ", 4);

    glutCreateMenu(menu_tetrahedron);
    glutAddSubMenu("Subdivion Level", subdivion);
    glutAddMenuEntry("Exit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
#pragma endregion

#pragma endregion

#pragma region 圓形
void circle(float x, float y, float radius, float glTranslatefX, float glTranslatefY, float glTranslatefZ, int red,int green, int blue)
{
    double p = 0.0;
    double q = 0.0;
    double angle = 1.0;

    glPushMatrix();
    glTranslatef(glTranslatefX, glTranslatefY, glTranslatefZ);
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(p, q);
    while (true)
    {
        if (angle <= 360)
        {
            p = x + sin(angle) * radius;
            q = y + cos(angle) * radius;
            glVertex2f(p, q);
        }
        else
        {
            break;
        }
        angle += 0.1;
    }
    glEnd();
    glPopMatrix();
}
void display_circle()
{
    // Middle Circle
    circle(0.0, 0.0, 0.25,
        0, 0, 0,
        1, 1, 1);

    // Up Circle
    circle(0.0, 0.0, 0.25,
        0, 0.5, 0,
        1, 0, 0);

    // Down Circle
    circle(0.0, 0.0, 0.25,
        0, -0.5, 0,
        0, 1, 0);

    // Up Left Circle
    circle(0.0, 0.0, 0.25,
        -0.425, 0.25, 0,
        0, 0, 1);

    // Up Right Circle
    circle(0.0, 0.0, 0.25,
        0.425, 0.25, 0,
        1, 1, 0);

    // Down Left Circle
    circle(0.0, 0.0, 0.25,
        -0.425, -0.25, 0,
        1, 0, 1);

    // Right Right Circle
    circle(0.0, 0.0, 0.25,
        0.425, -0.25, 0,
        0, 1, 1);

    glFlush(); // Render Now
}

#pragma endregion

#pragma region Square
void square() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);       //3%3=0
    glBegin(GL_POLYGON);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
    glFlush();
}
#pragma endregion

#pragma region keyboard
void keyboard(unsigned char key, int x, int y) {
    if (key == 'e' || key == 'E') {
        exit(0);
    }
    else if (key == '8') {
        if (glIsEnabled(GL_LIGHT0)) glDisable(GL_LIGHT0);
        else glEnable(GL_LIGHT0);
    }
    else if (key == '2') {
        if (glIsEnabled(GL_LIGHT1)) glDisable(GL_LIGHT1);
        else glEnable(GL_LIGHT1);
    }
    else if (key == '7' || key == '3') {
        if (glIsEnabled(GL_LIGHT2)) glDisable(GL_LIGHT2);
        else glEnable(GL_LIGHT2);
    }
    else if (key == '9' || key == '1') {
        if (glIsEnabled(GL_LIGHT3)) glDisable(GL_LIGHT3);
        else glEnable(GL_LIGHT3);
    }
    else if (key == '4') {
        if (glIsEnabled(GL_LIGHT4)) glDisable(GL_LIGHT4);
        else glEnable(GL_LIGHT4);
    }
    else if (key == '6') {
        if (glIsEnabled(GL_LIGHT5)) glDisable(GL_LIGHT5);
        else glEnable(GL_LIGHT5);
    }
    else if (key == 'z' || key =='Z' || key == 'j' || key == 'J') {     //stop
        walking = false;
        running = false;
    }
    else if (key == 'x' || key == 'X' || key == 'k' || key == 'K') {    //walk
        walking = true;
        running = false;
    }
    else if (key == 'c' || key == 'C' || key == 'l' || key == 'L') {    //run
        walking = false;
        running = true;
    }

    if (key == '+') {
        leg_run_speed_mul += 1;
    }
    else if (key == '-') {
        leg_run_speed_mul -= 1;
    }

    if (key == 'w' || key == 'W') {
        cameraElevation -= cameraSpeed * 10;
    }
    if (key == 'a' || key == 'A') {
        cameraAzimuth += cameraSpeed * 10;
    }
    if (key == 's' || key == 'S') {
        cameraElevation += cameraSpeed * 10;
    }
    if (key == 'd' || key == 'D') {
        cameraAzimuth -= cameraSpeed * 10;
    }
    if (key == 'u' || key == 'U') {
        cameraDistance += cameraSpeed * 50;  //太慢了所以多乘5
    }
    if (key == 'i' || key == 'I') {
        cameraDistance -= cameraSpeed * 50;  //太慢了所以多乘5
    }
    if (key == 'o' || key == 'O') {
        light_visible = !light_visible;
    }
    leg_run_speed_mul = fmax(leg_run_speed_mul, 1);
    leg_run_speed = leg_run_speed_mul * arm_run_speed * leg_run_max_angle / arm_run_max_angle;
    cameraElevation = fminf(fmaxf(cameraElevation, radian(-89)), radian(89));
    cameraDistance = fmaxf(cameraDistance, 1);      //camera距離最近為1
}
#pragma endregion

#pragma region Menu
void menu(int value) {
    switch (value) {
    case 0:
        exit(0);
        break;
    case 1:
        glEnable(GL_LIGHT0);
        break;
    case 2:
        glDisable(GL_LIGHT0);
        break;
    case 3:
        glEnable(GL_LIGHT1);
        break;
    case 4:
        glDisable(GL_LIGHT1);
        break;
    case 5:
        glEnable(GL_LIGHT2);
        break;
    case 6:
        glDisable(GL_LIGHT2);
        break;
    case 7:
        glEnable(GL_LIGHT3);
        break;
    case 8:
        glDisable(GL_LIGHT3);
        break;
    case 9:
        glEnable(GL_LIGHT4);
        break;
    case 10:
        glDisable(GL_LIGHT4);
        break;
    case 11:
        glEnable(GL_LIGHT5);
        break;
    case 12:
        glDisable(GL_LIGHT5);
        break;
    case 13:
        walking = false;
        running = false;
        break;
    case 14:
        walking = true;
        running = false;
        break;
    case 15:
        walking = false;
        running = true;
        break;
    case 16:
        leg_run_speed_mul += 1;
        break;
    case 17:
        leg_run_speed_mul -= 1;
        break;
    case 18:
        leg_run_speed_mul = 1;
        break;
    case 19:
        light_visible = !light_visible;
        break;
    default:
        break;
    }
    if (leg_run_speed_mul <= 1) leg_run_speed_mul = 1;
    leg_run_speed = leg_run_speed_mul * arm_run_speed * leg_run_max_angle / arm_run_max_angle;
}
void createMenu() {
    int light_switch0 = glutCreateMenu(menu);
    glutAddMenuEntry("ON    ", 1);
    glutAddMenuEntry("OFF   ", 2);

    int light_switch1 = glutCreateMenu(menu);
    glutAddMenuEntry("ON    ", 3);
    glutAddMenuEntry("OFF   ", 4);

    int light_switch2 = glutCreateMenu(menu);
    glutAddMenuEntry("ON    ", 5);
    glutAddMenuEntry("OFF   ", 6);

    int light_switch3 = glutCreateMenu(menu);
    glutAddMenuEntry("ON    ", 7);
    glutAddMenuEntry("OFF   ", 8);

    int light_switch4 = glutCreateMenu(menu);
    glutAddMenuEntry("ON    ", 9);
    glutAddMenuEntry("OFF   ", 10);

    int light_switch5 = glutCreateMenu(menu);
    glutAddMenuEntry("ON    ", 11);
    glutAddMenuEntry("OFF   ", 12);

    int lights = glutCreateMenu(menu);
    glutAddSubMenu("LIGHT-0", light_switch0);
    glutAddSubMenu("LIGHT-1", light_switch1);
    glutAddSubMenu("LIGHT-2", light_switch2);
    glutAddSubMenu("LIGHT-3", light_switch3);
    glutAddSubMenu("LIGHT-4", light_switch4);
    glutAddSubMenu("LIGHT-5", light_switch5);

    int movements = glutCreateMenu(menu);
    glutAddMenuEntry("STOP",13);
    glutAddMenuEntry("WALK", 14);
    glutAddMenuEntry("RUN", 15);

    int run_speed = glutCreateMenu(menu);
    glutAddMenuEntry("FASTER", 16);
    glutAddMenuEntry("SLOWER", 17);
    glutAddMenuEntry("NORMAL", 18);

    glutCreateMenu(menu);
    glutAddSubMenu("LIGHTING CONTROL", lights);
    glutAddSubMenu("ROBOT ACTIONS", movements);
    glutAddSubMenu("RUNNING SPEED", run_speed);
    glutAddMenuEntry("VISUALIZE LIGHT", 19);
    glutAddMenuEntry("Exit", 0);
}
#pragma endregion

#pragma region 滑鼠控制(視角)
void mouseMotion(int x, int y) {

    int deltaX = x - lastX;
    int deltaY = y - lastY;

    if (leftButtonPressed && rightButtonPressed) {  //一起按調整距離遠近
        cameraDistance += deltaY * cameraSpeed * 5;  //太慢了所以多乘5
        cameraDistance = fmaxf(cameraDistance, 1);      //距離最近為1
    }
    else if (leftButtonPressed) {
        // 調整相機的方位角和仰角
        cameraAzimuth -= deltaX * cameraSpeed;
        cameraElevation += deltaY * cameraSpeed;
        glutDetachMenu(GLUT_RIGHT_BUTTON);
        // 限制仰角範圍，避免相機翻轉
        //std::cout << "仰角 : " << cameraElevation << std::endl;
        cameraElevation = fminf(fmaxf(cameraElevation, radian(-89)) ,radian(89));
    }
    else if (!(leftButtonPressed && rightButtonPressed) || rightButtonPressed) {
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }

    lastX = x;
    lastY = y;
    glutPostRedisplay();
}
void mouseButton(int button, int state, int x, int y) {

    switch (button) {
    case GLUT_LEFT_BUTTON:
        leftButtonPressed = (state == GLUT_DOWN);
        break;
    case GLUT_RIGHT_BUTTON:
        rightButtonPressed = (state == GLUT_DOWN);
        break;
    case 3:
        cameraDistance += -1 * 0.5;
        break;
    case 4:
        cameraDistance += 1 * 0.5;
        break;
    default:
        break;
    }

    cameraDistance = fmaxf(cameraDistance, 1);
    lastX = x;
    lastY = y;
    glutPostRedisplay();
}
#pragma endregion
GLfloat test = 3;
GLfloat n = 0;
#pragma region 計時器
void refresh_timer(int value) {

    light_Elevation2 -= light_speed;
    light_Elevation3 += light_speed;

    if (walking) {
        //身體起伏
        if (head_y >= (initial_head_y + max_ups_and_downs_walk)) ups_and_downs_walk_speed = -abs(ups_and_downs_walk_speed);
        else if (head_y <= (initial_head_y - max_ups_and_downs_walk)) ups_and_downs_walk_speed = abs(ups_and_downs_walk_speed);
        head_y += ups_and_downs_walk_speed;
        //手擺動
        if (shoulder >= arm_walk_max_angle) arm_walk_speed = -abs(arm_walk_speed);
        else if (shoulder <= -arm_walk_max_angle ) arm_walk_speed = abs(arm_walk_speed);
        shoulder += arm_walk_speed;
        //腳擺動
        if (leg_angle <= -leg_walk_max_angle) leg_walk_speed = abs(leg_walk_speed);
        else if(leg_angle >= leg_walk_max_angle) leg_walk_speed = -abs(leg_walk_speed);
        leg_angle += leg_walk_speed;
    }
    else if (running) {
        //身體起伏
        if (head_y >= (initial_head_y + max_ups_and_downs_run)) ups_and_downs_run_speed = -abs(ups_and_downs_run_speed);
        else if (head_y <= (initial_head_y - max_ups_and_downs_run)) ups_and_downs_run_speed = abs(ups_and_downs_run_speed);
        head_y += ups_and_downs_run_speed;
        //手擺動
        if (shoulder >= arm_run_max_angle) arm_run_speed = -abs(arm_run_speed);
        else if(shoulder <= -arm_run_max_angle) arm_run_speed = abs(arm_run_speed);
        shoulder += arm_run_speed;
        //腳擺動
        if (leg_angle <= -leg_run_max_angle ) leg_run_speed = abs(leg_run_speed);
        else if(leg_angle >= leg_run_max_angle) leg_run_speed = -abs(leg_run_speed);
        leg_angle += leg_run_speed;
    }
    else {
        shoulder = 0;
        leg_angle = 0;
        arm_walk_speed = abs(arm_walk_speed);
        leg_walk_speed = abs(leg_walk_speed);
        arm_run_speed = abs(arm_run_speed);
        leg_run_speed = abs(leg_run_speed);
        ups_and_downs_walk_speed = abs(ups_and_downs_walk_speed);
        ups_and_downs_run_speed = abs(ups_and_downs_run_speed);
    }

    if (test <= 0) n = (0.7 / 1000);
    else if (test >= 3) n = -(0.7 / 1000);
    test += n;

    glutPostRedisplay();
    glutTimerFunc(1, refresh_timer, 0);
}
#pragma endregion

void draw_testing() {
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_gold);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_gold);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_gold);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_gold);
    glRotatef(45, 0.0, 1.0, 0.0);               //轉45度，後面是法向量
    glutSolidTeapot(0.5);
    glPopAttrib();
    glPopMatrix();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);    //red
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_bronze);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_bronze);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_bronze);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_bronze);
    glutSolidSphere(0.25, 50, 50);
    glPopAttrib();
    glPopMatrix();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);    //green
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_emerald);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_emerald);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_emerald);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_emerald);
    glutSolidSphere(0.25, 50, 50);
    glPopAttrib();
    glPopMatrix();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);    //blue
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_cyan_rubber);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_cyan_rubber);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_cyan_rubber);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_cyan_rubber);
    glutSolidSphere(0.25, 50, 50);
    glPopAttrib();
    glPopMatrix();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glTranslatef(0.0, 0.0, test);    //blue
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_cyan_rubber);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_cyan_rubber);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_cyan_rubber);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_cyan_rubber);
    glutSolidSphere(0.25, 50, 50);
    glPopAttrib();
    glPopMatrix();
}
void draw_face(float x_, float y_, float z_, float head_size) {

    // eyes
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.01 + head_size / 2);           //x和y要去改center
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_eye);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_eye);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_eye);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_no);
    float eye_disatnce = 0.5f;
    float centerX = x_ - eye_disatnce / 2.0;
    float centerY = y_ + eye_disatnce / 2.0;
    float radius = 0.1f;                                //眼睛大小
    float eye_socket = 0.03;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(centerX, centerY, z_);  // 圓心
    for (float angle = 0.0; angle <= 2 * PI; angle += 0.005) {
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex3f(x, y, z_);
    }
    glEnd();
    glPopAttrib();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_eye);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_eye_white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_eye);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_no);
    glMaterialfv(GL_FRONT, GL_EMISSION, diffuse_eye_white);
    glTranslatef(0.0, 0.0, 0.0001);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(centerX, centerY, z_);  // 圓心
    for (float angle = 0.0; angle <= 2 * PI; angle += 0.005) {
        float x = centerX + (radius - eye_socket) * cos(angle);
        float y = centerY + (radius - eye_socket) * sin(angle);
        glVertex3f(x, y, z_);
    }
    glEnd();
    glPopAttrib();
    glTranslatef(0.0, 0.0, -0.0001);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_eye);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_eye);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_eye);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_no);
    glBegin(GL_TRIANGLE_FAN);
    centerX += eye_disatnce;
    glVertex3f(centerX, centerY, z_);  // 圓心
    for (float angle = 0.0; angle <= 2 * PI; angle += 0.005) {
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex3f(x, y, z_);
    }
    glEnd();
    glPopAttrib();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_eye);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_eye_white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_eye);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_no);
    glMaterialfv(GL_FRONT, GL_EMISSION, diffuse_eye_white);
    glTranslatef(0.0, 0.0, 0.0001);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(centerX, centerY, z_);  // 圓心
    for (float angle = 0.0; angle <= 2 * PI; angle += 0.005) {
        float x = centerX + (radius - eye_socket) * cos(angle);
        float y = centerY + (radius - eye_socket) * sin(angle);
        glVertex3f(x, y, z_);
    }
    glEnd();
    glPopAttrib();
    glPopMatrix();

    // mouth
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    float mounth_size = 0.1;
    float mouth_yoffset = -(head_size / 4);
    glTranslatef(0, 0, 0.01 + head_size / 2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_mouth);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_mouth);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_mouth);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine_no);
    glMaterialfv(GL_FRONT, GL_EMISSION, diffuse_mouth);
    glBegin(GL_TRIANGLES);
    glVertex2f(x_ - mounth_size, y_  + mouth_yoffset - mounth_size);
    glVertex2f(x_ + mounth_size, y_ + mouth_yoffset - mounth_size);
    glVertex2f(x_, y_ + mouth_yoffset  + mounth_size);
    glEnd();
    glPopAttrib();
    glPopMatrix();
}
void draw_head(float x = 0.0,float y = head_y - head_size / 2, float z = 0.0) {

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glTranslatef(x, y, z);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_cyan_rubber);
    glutSolidCube(head_size);
    glPopAttrib();
    glPopMatrix();

    draw_face(x, y, z, head_size);
}
void draw_body() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_cyan_rubber);
    
    
    float cube_size_d2 = body_height / 3;          //上到下用3個cube組成
    body_width = 3 * cube_size_d2;
    bofy_depth = 2 * cube_size_d2;
    //前胸
    glTranslatef(cube_size_d2, head_y - head_size - body_space_with_head - cube_size_d2 / 2, cube_size_d2 / 2);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(-cube_size_d2, 2 * cube_size_d2, 0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(-cube_size_d2, 2 * cube_size_d2, 0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);

    //後背
    glTranslatef(0.0, 2 * cube_size_d2, -cube_size_d2);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(cube_size_d2, 2 * cube_size_d2, 0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(cube_size_d2, 2 * cube_size_d2, 0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    glTranslatef(0.0, -cube_size_d2, 0.0);
    glutSolidCube(cube_size_d2);
    
    glPopAttrib();
    glPopMatrix();
}
void draw_right_arm() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_cyan_rubber);

    glTranslatef(-(body_width / 2 + arm_space_with_body), body_y, 0.0);   //大臂關節點
    glTranslatef(-arm_thickness / 2, -arm_thickness / 2, 0.0);
    glRotatef(shoulder, 1.0, 0.0, 0.0);
    for (int i = 0; i < half_arm_length / arm_thickness; i++) {
        glutSolidCube(arm_thickness);
        glTranslatef(0.0, -arm_thickness, 0.0);
    }

    float elbow = (shoulder >= -3.0) ? -3.0 : shoulder * 1.5;
    glRotatef(elbow, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -bigarm_space_with_smallarm, 0.0);   //畫小臂
    for (int i = 0; i < half_arm_length / arm_thickness; i++) {
        glutSolidCube(arm_thickness);
        glTranslatef(0.0, -arm_thickness, 0.0);
    }

    glPopAttrib();
    glPopMatrix();
}
void draw_left_arm(){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_cyan_rubber);

    glTranslatef(body_width / 2 + arm_space_with_body, body_y, 0.0);   //大臂關節點
    glTranslatef(arm_thickness / 2, -arm_thickness / 2, 0.0);
    glRotatef(shoulder, -1.0, 0.0, 0.0);
    for (int i = 0; i < half_arm_length / arm_thickness; i++) {
        glutSolidCube(arm_thickness);
        glTranslatef(0.0, -arm_thickness, 0.0);
    }

    float elbow = (shoulder <= 3.0) ? 3.0 : shoulder * 1.5;
    glRotatef(elbow, -1.0, 0.0, 0.0);
    glTranslatef(0.0, -bigarm_space_with_smallarm, 0.0);   //畫小臂
    for (int i = 0; i < half_arm_length / arm_thickness; i++) {
        glutSolidCube(arm_thickness);
        glTranslatef(0.0, -arm_thickness, 0.0);
    }

    glPopAttrib();
    glPopMatrix();
}
void draw_right_leg() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_cyan_rubber);

    glTranslatef(-(space_between_leg + leg_thickness), leg_y, 0.0);   //大腿關節點
    glTranslatef(0.0, -leg_thickness / 2, 0.0);
    glRotatef(leg_angle, -1.0, 0.0, 0.0);
    for (int i = 0; i < half_leg_length / leg_thickness; i++) {
        glutSolidCube(leg_thickness);
        glTranslatef(0.0, -leg_thickness, 0.0);
    }

    if (walking) {
        if (leg_walk_speed > 0 && leg_angle < 0) {
            calf_rorate_right -= leg_walk_speed * 2;
        }
        else if (leg_walk_speed > 0 && leg_angle >= 0) {
            if(right_leg_begin_walk) calf_rorate_right -= leg_walk_speed * 2;
            else calf_rorate_right += leg_walk_speed * 2;
        }
        else if (leg_walk_speed <= 0 && leg_angle > 0) {
            calf_rorate_right -= leg_walk_speed * 2 ;
            if (calf_rorate_right >= 0) calf_rorate_right = 0;
            right_leg_begin_walk = false;
        }
        else if (leg_walk_speed <= 0 && leg_angle <= 0) {
            calf_rorate_right = (leg_angle >= 3.0) ? 3.0 : leg_angle * 1.5;
        }
    }
    else if (running) {
        if (leg_run_speed > 0 && leg_angle <= 0) {
            calf_rorate_right -= leg_run_speed;
        }
        else if (leg_run_speed > 0 && leg_angle > 0) {
            if(right_leg_begin_run) calf_rorate_right -= leg_run_speed;
            else calf_rorate_right += leg_run_speed;
        }
        else if (leg_run_speed <= 0 && leg_angle >= 0) {
            calf_rorate_right -= leg_run_speed;
            if (calf_rorate_right >= 0) calf_rorate_right = 0;
            right_leg_begin_run = false;
        }
        else if (leg_run_speed <= 0 && leg_angle < 0) {
            calf_rorate_right = (leg_angle >= 3.0) ? 3.0 : leg_angle * 0.5;
            if (calf_rorate_right >= 40) calf_rorate_right = 40;
        }
    }
    else {
        calf_rorate_right = 0;
        right_leg_begin_walk = true;
        right_leg_begin_run = true;
    }
    
    glRotatef(calf_rorate_right, -1.0, 0.0, 0.0);
    glTranslatef(0.0, -bigleg_space_with_smallleg, 0.0);   //畫小腿
    for (int i = 0; i < half_leg_length / leg_thickness; i++) {
        glutSolidCube(leg_thickness);
        glTranslatef(0.0, -leg_thickness, 0.0);
    }
    glPopAttrib();
    glPopMatrix();
}
void draw_left_leg() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_cyan_rubber);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_cyan_rubber);

    glTranslatef((space_between_leg + leg_thickness), leg_y, 0.0);   //大腿關節點
    glTranslatef(0.0, -leg_thickness / 2, 0.0);
    glRotatef(leg_angle, 1.0, 0.0, 0.0);
    for (int i = 0; i < half_leg_length / leg_thickness; i++) {
        glutSolidCube(leg_thickness);
        glTranslatef(0.0, -leg_thickness, 0.0);
    }

    if (walking) {
        if (leg_walk_speed >= 0 && leg_angle >= 0) {
            calf_rorate_left = (leg_angle <= -3.0) ? -3.0 : leg_angle * 1.5;
        }
        else if (leg_walk_speed >= 0 && leg_angle < 0) {
            calf_rorate_left -= leg_walk_speed * 2;
            if (calf_rorate_left <= 0) calf_rorate_left = 0;
        }
        else if (leg_walk_speed < 0 && leg_angle <= 0) {
            calf_rorate_left += leg_walk_speed * 2;
        }
        else if (leg_walk_speed < 0 && leg_angle > 0) {
            calf_rorate_left -= leg_walk_speed * 2;
        }
    }
    else if (running) {
        if (leg_run_speed >= 0 && leg_angle >= 0) {
            calf_rorate_left = (leg_angle <= -3.0) ? -3.0 : leg_angle * 0.5;
            if (calf_rorate_left >= 40) calf_rorate_left = 40;
        }
        else if (leg_run_speed >= 0 && leg_angle < 0) {
            calf_rorate_left -= leg_run_speed;
            if (calf_rorate_left <= 0) calf_rorate_left = 0;
        }
        else if (leg_run_speed < 0 && leg_angle <= 0) {
            calf_rorate_left += leg_run_speed;
        }
        else if (leg_run_speed < 0 && leg_angle > 0) {
            calf_rorate_left -= leg_run_speed * 1.0;
        }
    }
    else {
        calf_rorate_left = 0;
    }
    glRotatef(calf_rorate_left, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -bigleg_space_with_smallleg, 0.0);   //畫小腿
    for (int i = 0; i < half_leg_length / leg_thickness; i++) {
        glutSolidCube(leg_thickness);
        glTranslatef(0.0, -leg_thickness, 0.0);
    }
    glPopAttrib();
    glPopMatrix();
}
void draw_robot() {
    draw_head();
    body_y = head_y - head_size - body_space_with_head;
    draw_body();
    leg_y = body_y - body_height - leg_space_with_body;
    draw_right_arm();
    draw_left_arm();
    draw_right_leg();
    draw_left_leg();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    #pragma region Camera
    float new_cameraElevation = cameraElevation - radian(90);
    float cameraX = cameraDistance * sin(new_cameraElevation) * sin(cameraAzimuth);
    float cameraY = cameraDistance * cos(new_cameraElevation);
    float cameraZ = cameraDistance * sin(new_cameraElevation) * cos(cameraAzimuth);

    //cameraDistance *= 4;
    //glOrtho(-cameraDistance, cameraDistance, -cameraDistance, cameraDistance, -cameraDistance, cameraDistance);
    //cameraDistance /= 4;
    //GLfloat u[] = { -cameraX , cameraDistance - cameraY  , -cameraZ };
    //GLfloat v[] = { -cameraX ,-cameraY ,-cameraZ };
    //GLfloat temp[] = {0,0,0};
    //int tt = 0;
    //for (int i = 0; i < 3; i++)  tt += u[i] * v[i];
    //tt /= cameraDistance * cameraDistance;
    //for (int i = 0; i < 3; i++) temp[i] = u[i] - (tt * v[i]);

    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //std::cout << std::endl <<"camera : " << cameraX << "\t" << cameraY << "\t" << cameraZ << std::endl;
    //std::cout << temp[0]<<"\t" << temp[1] << "\t" << temp[2] << std::endl;
    #pragma endregion

    #pragma region 燈光


    if (glIsEnabled(GL_LIGHT0) && light_visible) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_silver);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_silver);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_silver);
        glMaterialfv(GL_FRONT, GL_SHININESS, shine_silver);
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glPushMatrix();
        glTranslatef(light_position0[0], light_position0[1], light_position0[2]);
        glutSolidSphere(0.05, 100, 100);
        glPopAttrib();
        glPopMatrix();
    }
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    if (glIsEnabled(GL_LIGHT1) && light_visible) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_silver);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_silver);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_silver);
        glMaterialfv(GL_FRONT, GL_SHININESS, shine_silver);
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glPushMatrix();
        glTranslatef(light_position1[0], light_position1[1], light_position1[2]);
        glutSolidSphere(0.05, 100, 100);
        glPopAttrib();
        glPopMatrix();
    }
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    
    float lightX = light23radian * sin(radian(light_Elevation2)) * sin(radian(light_Azimuth2));
    float lightY = light23radian * cos(radian(light_Elevation2));
    float lightZ = light23radian * sin(radian(light_Elevation2)) * cos(radian(light_Azimuth2));
    light_position2[0] = lightX;
    light_position2[1] = lightY;
    light_position2[2] = lightZ;
    if (glIsEnabled(GL_LIGHT2) && light_visible) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        glTranslatef(lightX, lightY, lightZ);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_silver);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_silver);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_silver);
        glMaterialfv(GL_FRONT, GL_SHININESS, shine_silver);
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glutSolidSphere(0.05, 100, 100);
        glPopAttrib();
        glPopMatrix();
    }
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);

    lightX = light23radian * -sin(radian(light_Elevation3)) * sin(radian(light_Azimuth3));
    lightY = light23radian * -cos(radian(light_Elevation3));
    lightZ = light23radian * -sin(radian(light_Elevation3)) * cos(radian(light_Azimuth3));
    light_position3[0] = lightX;
    light_position3[1] = lightY;
    light_position3[2] = lightZ;
    if (glIsEnabled(GL_LIGHT3) && light_visible) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        glTranslatef(lightX, lightY, lightZ);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_silver);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_silver);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_silver);
        glMaterialfv(GL_FRONT, GL_SHININESS, shine_silver);
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glutSolidSphere(0.05, 100, 100);
        glPopAttrib();
        glPopMatrix();
    }


    glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
    glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);

    if (glIsEnabled(GL_LIGHT4) && light_visible) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_silver);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_silver);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_silver);
        glMaterialfv(GL_FRONT, GL_SHININESS, shine_silver);
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glPushMatrix();
        glTranslatef(light_position4[0], light_position4[1], light_position4[2]);
        glutSolidSphere(0.05, 100, 100);
        glPopAttrib();
        glPopMatrix();
    }
    glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
    glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
    glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);

    if (glIsEnabled(GL_LIGHT5) && light_visible) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_silver);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_silver);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_silver);
        glMaterialfv(GL_FRONT, GL_SHININESS, shine_silver);
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glPushMatrix();
        glTranslatef(light_position5[0], light_position5[1], light_position5[2]);
        glutSolidSphere(0.05, 100, 100);
        glPopAttrib();
        glPopMatrix();
    }
    glLightfv(GL_LIGHT5, GL_POSITION, light_position5);
    glLightfv(GL_LIGHT5, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, light_diffuse5);
    glLightfv(GL_LIGHT5, GL_SPECULAR, light_specular);
    #pragma endregion
    
    //draw_testing();
    draw_robot();

    glutSwapBuffers();
}
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double)w / (double)h, 1, 50);
}
void initialize() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glShadeModel(GL_SMOOTH);
}
int main(int argc, char* argv[])
{
    /*glutInit(&argc, argv); // Initialize GLUT                 //圓形的
    int x = 512, y = 512;  // x and y value
    glutInitWindowPosition(
        (int)(glutGet(GLUT_SCREEN_WIDTH) - x) / 2,
        (int)(glutGet(GLUT_SCREEN_HEIGHT) - y) / 2); // Position the window's center
    glutInitWindowSize(x, y);                        // Set the window's initial width & height
    glutCreateWindow("Flower Shape Circle");         // Create a window with the given title
    initialize();                                    // Initializing
    glutDisplayFunc(display_circle);                        // Register display callback handler for window re-paint
    glutMainLoop();                                  // Enter the event-processing loop
    */

    /*
    glutInit(&argc, argv);      //square 作業一
    glutInitDisplayMode(GLUT_SINGLE);
    initialize();
    glutCreateWindow("S10959013");
    glutDisplayFunc(square);
    glutMainLoop();
    */

    /*
    glutInit(&argc, argv);                //tetrahedron
    initialize();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("S10959013");
    createMenu_tetrahedron();
    glutDisplayFunc(display_tetrahedron);
    glutKeyboardFunc(keyboard);         //exit
    glEnable(GL_DEPTH_TEST);
    */

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("S10959013");
    initialize();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);
    createMenu();
    glutTimerFunc(1, refresh_timer, 0);

    glutMainLoop();
    return 0;
}