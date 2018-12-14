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
    level_t current_level = levels[level];

    // place ball at starting point
    point_t starting_point = current_level.start;

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);
    circleShape.m_radius = 0.2f;

    b2BodyDef circleBodyDef;
    circleBodyDef.type = b2_dynamicBody;
    circleBodyDef.position.Set(starting_point.x, starting_point.y);
    b2Body* circleBody = world->CreateBody(&circleBodyDef);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    circleBody->CreateFixture(&fixtureDef);

    // place square at goal point
    point_t goal_point = current_level.end;

    b2PolygonShape boxShape;
    b2Vec2 *box_vertices = new b2Vec2[4];
    box_vertices[0].Set(goal_point.x, goal_point.y+0.2);
    box_vertices[1].Set(goal_point.x, goal_point.y);
    box_vertices[2].Set(goal_point.x+0.2, goal_point.y);
    box_vertices[3].Set(goal_point.x+0.2, goal_point.y+0.2);
    boxShape.Set(box_vertices, 4);

    b2BodyDef squareBodyDef;
    squareBodyDef.type = b2_staticBody;
    squareBodyDef.position.Set(goal_point.x, goal_point.y);
    squareBodyDef.angle = 0;
    b2Body* squareBody = world->CreateBody(&squareBodyDef);

    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    squareBody->CreateFixture(&boxFixtureDef);

    for (int i = 0; i < (int) current_level.num_polygons; i++) {
    // body stuff
    poly_t new_polygon = current_level.polygons[i];
    b2BodyDef polyBodyDef;

    if (new_polygon.is_dynamic) {
        polyBodyDef.type = b2_dynamicBody;
    } else {
        polyBodyDef.type = b2_staticBody;
    }
    polyBodyDef.position.Set(new_polygon.position.x, new_polygon.position.y);

    // shape stuff
    b2PolygonShape polygonShape;
    b2Vec2 *vertices = new b2Vec2[new_polygon.num_verts];
    for (int j = 0; j < (int) new_polygon.num_verts; j++) {
        vertices[j].Set(new_polygon.verts[j].x, new_polygon.verts[j].y);
    }
    polygonShape.Set(vertices, new_polygon.num_verts);

    // fixture stuff
    b2FixtureDef polygonFixtureDef;
    polygonFixtureDef.shape = &polygonShape;

    // finishing up
    b2Body* polygonBody = world->CreateBody(&polyBodyDef);
    polygonBody->CreateFixture(&polygonFixtureDef);

    }
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

    // Physics stuff
    world->Step(timeStep, velocityIterations, positionIterations);

    b2Body *b = world->GetBodyList();
    while (b != NULL) {

        GLuint buffer[1];
        glGenBuffers(1, buffer);
        glBindBuffer(GL_ARRAY_BUFFER, *buffer);

        b2Fixture* f = b->GetFixtureList();
        while (f != NULL) {
            if (f->GetType() == b2Shape::e_circle) {
                glColor3f(1.0, 0, 0);
                int num_segments = 36;
                b2CircleShape* circle = (b2CircleShape*)f->GetShape();

                // circle stuff
                b2Vec2 position = b->GetPosition();
                printf("%f, %f\n", position.x, position.y);
                float *vertices = new float[(num_segments + 2) * 2];
                float radius_circle = circle->m_radius;
                circle_array(vertices, position.x, position.y, radius_circle, num_segments);

                // GL Load circle stuff
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (num_segments + 2) * 2,
                             vertices, GL_STREAM_DRAW);
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(2, GL_FLOAT, 0, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, (num_segments + 2));
            } else {
                glColor3f(0, 1.0, 0);
                //draw polygon shit
                b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();

                printf("Vertices: %i\n", poly->GetVertexCount());

                float *vertices = new float[poly->GetVertexCount() * 2];
                for (int i = 0; i < poly->GetVertexCount(); i++) {
                    b2Vec2 corner = poly->GetVertex(i);
                    vertices[i] = corner.x;
                    vertices[i+1] = corner.y;
                    printf("corners: %f, %f\n", vertices[i], vertices[i+1]);
                }

                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * poly->GetVertexCount() * 2,
                             vertices, GL_STATIC_DRAW);
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(2, GL_FLOAT, 0, 0);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, poly->GetVertexCount());
            }

            f = f->GetNext();
        }

        // VBO drawing stuff
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, buffer);

        b = b->GetNext();
    }

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
 * Called when the user presses a key.glEnableClientState
 */
void key_pressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // Esc
        case 'q':
            exit(0);
            break;
        case 'r':
            load_world(0);
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
