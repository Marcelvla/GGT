/* Computer Graphics and Game Technology, Assignment Box2D game
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <cstdio>

#ifdef __APPLE__
#include <OpenGL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <Box2D/Box2D.h>

#include "levels.h"

unsigned int reso_x = 800, reso_y = 600; // Window size in pixels
const float world_x = 8.f, world_y = 6.f; // Level (world) size in meters

int last_time;
int frame_count;

// Information about the levels loaded from files will be available in these.
unsigned int num_levels;
level_t *levels;

b2World* world;
b2Body* circleBody;

float timeStep = 1.0/60.0;
int velocityIterations = 6;
int positionIterations = 2;

/*
int circleEdgePts = 36;
float twoPi = 2.0f * 3.14159f;
glBegin(GL_TRIANGLE_FAN);
glVertex2f(position.x, position.y);
*/

void circle_array(float* vertices, float start_x, float start_y, float r, int num_segments)
{
    vertices[0] = start_x;
    vertices[1] = start_y; // middle point
    for(int i = 0; i <= num_segments; i++)
    {
        // Calculate angle, x and y component, then add to array
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        //printf("%f%f\n", start_x + x, start_y + y);
        vertices[(i + 1) * 2] = start_x + x;
        vertices[(i + 1) * 2 + 1] = start_y + y;
    }
    //printf("%s\n", "end");
}

/*
 * Load a given world, i.e. read the world from the `levels' data structure and
 * convert it into a Box2D world.
 */
void load_world(unsigned int level)
{
    if (level >= num_levels)
    {
        // Note that level is unsigned but we still use %d so -1 is shown as
        // such.
        printf("Warning: level %d does not exist.\n", level);
        return;
    }


    // Create a Box2D world and populate it with all bodies for this level
    // (including the ball).
    b2Vec2 gravity(0.0f, -9.81f);
    world = new b2World(gravity);

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);
    circleShape.m_radius = 0.2f;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(4.0f, 3.0f);
    circleBody = world->CreateBody(&bodyDef);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    circleBody->CreateFixture(&fixtureDef);

    // Configure step method

}

/*
 * Called when we should redraw the scene (i.e. every frame).
 * It will show the current framerate in the window title.
 */
void draw(void)
{
    int time = glutGet(GLUT_ELAPSED_TIME);
    int frametime = time - last_time;
    frame_count++;

    // Clear the buffer
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0, 0);


    // Physics stuff
    world->Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = circleBody->GetPosition();
    float angle = circleBody->GetAngle();
    // printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

    // circle stuff
    int num_segments = 36;
    float *vertices = new float[(num_segments + 2) * 2];
    float radius_circle = 0.2f;
    circle_array(vertices, position.x, position.y, radius_circle, num_segments);

    // VBO initializing stuff
    GLuint buffer[1];
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (num_segments + 2) * 2,
                 vertices, GL_STREAM_DRAW);

    // VBO drawing stuff
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (num_segments + 2));
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, buffer);

    // Show rendered frame
    glutSwapBuffers();


    // Display fps in window title.
    if (frametime >= 1000)
    {
        char window_title[128];
        snprintf(window_title, 128,
                "Box2D: %f fps, level %d/%d",
                frame_count / (frametime / 1000.f), -1, num_levels);
        glutSetWindowTitle(window_title);
        last_time = time;
        frame_count = 0;
    }
}

/*
 * Called when window is resized. We inform OpenGL about this, and save this
 * for future reference.
 */
void resize_window(int width, int height)
{
    glViewport(0, 0, width, height);
    reso_x = width;
    reso_y = height;
}

/*
 * Called when the user presses a key.
 */
void key_pressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // Esc
        case 'q':
            exit(0);
            break;
        case 'd':
        // Add any keys you want to use, either for debugging or gameplay.
        default:
            break;
    }
}

/*
 * Called when the user clicked (or released) a mouse buttons inside the window.
 */
void mouse_clicked(int button, int state, int x, int y)
{

}

/*
 * Called when the mouse is moved to a certain given position.
 */
void mouse_moved(int x, int y)
{

}


int main(int argc, char **argv)
{
    // Create an OpenGL context and a GLUT window.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(reso_x, reso_y);
    glutCreateWindow("Box2D");

    glewInit();

    // Bind all GLUT events do callback function.
    glutDisplayFunc(&draw);
    glutIdleFunc(&draw);
    glutReshapeFunc(&resize_window);
    glutKeyboardFunc(&key_pressed);
    glutMouseFunc(&mouse_clicked);
    glutMotionFunc(&mouse_moved);
    glutPassiveMotionFunc(&mouse_moved);

    // Initialise the matrices so we have an orthogonal world with the same size
    // as the levels, and no other transformations.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, world_x, 0, world_y, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Read the levels into a bunch of structs.
    num_levels = load_levels(&levels);
    printf("Loaded %d levels.\n", num_levels);

    // Load the first level (i.e. create all Box2D stuff).
    load_world(0);

    last_time = glutGet(GLUT_ELAPSED_TIME);
    frame_count = 0;
    glutMainLoop();

    return 0;
}
