#ifndef CSE168_MIRO_GLUT_WINDOW_H_INCLUDED
#define CSE168_MIRO_GLUT_WINDOW_H_INCLUDED

class MiroWindow
{
public:
    MiroWindow(int * argc, char* argv[]);
    
    void mainLoop();
    
    // GLUT Event handlers
    void display();
    void reshape(int x, int y);
    void keyboard(unsigned char key, int x, int y);
    void mouse(int btn, int state, int x, int y);
    void motion(int x, int y);
    
protected:
    float m_scaleFact;
    int   m_activeButton;
    int   m_mouseX, m_mouseY;
};

#endif // CSE168_MIRO_GLUT_WINDOW_H_INCLUDED


