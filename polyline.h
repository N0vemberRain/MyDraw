#ifndef POLYLINE_H
#define POLYLINE_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

#include "types.h"

class Polyline : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    enum { Type = PolylineType };
    explicit Polyline(const QVector<QPointF> &points);
    ~Polyline() {}

    int type() const { return Type; }
    void setMouseEvent(QGraphicsSceneMouseEvent *pe) {
        if(pe->type() == QEvent::GraphicsSceneMouseMove) {
            mouseMoveEvent(pe);
        }
        if(pe->type() == QEvent::GraphicsSceneMousePress) {
            mousePressEvent(pe);
        }
    }

    void setData(const QStringList &data);
    QStringList getData() const;
    QRectF getRect() const {
        return boundingRect();
    }
    void setShear(const double horizontal, const double vertical);

public:
    void select(bool state);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    void updateTransform();
    QPainterPath mPoints;
    QVector<QLineF> mPolyline;
    QPen *mPen;

    QPointF mPreviousPoint;
    double mHorizontalShear;
    double mVerticalShear;
};














class MoveItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MoveItem(QObject *parent = 0);
    ~MoveItem();

    void setup() {
        this->setPos(50, 50);
    }

    void setMouseEvent(QGraphicsSceneMouseEvent *event);
signals:

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
};


#endif // POLYLINE_H

/*

#include <iostream>
using namespace std;
#include "vgl.h"
#include "LoadShaders.h"
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
const GLuint  NumVertices = 6;
//--------------------------------------------------------------------
//
// init
//
void init(void)
{
    static const GLfloat vertices[NumVertices][2] =
    {
        { -0.90, -0.90 },  // Triangle 1
        {  0.85, -0.90 },
        { -0.90,  0.85 },
        {  0.90, -0.85 },  // Triangle 2
        {  0.90,  0.90 },
        { -0.85,  0.90 }
    };
    glCreateBuffers(NumBuffers, Buffers);
    glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices),
                         vertices, 0);
    ShaderInfo  shaders[] = {
        { GL_VERTEX_SHADER, "triangles.vert" },
        { GL_FRAGMENT_SHADER, "triangles.frag" },
        { GL_NONE, NULL }
    };
    GLuint program = LoadShaders(shaders);
    glUseProgram(program);
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}
//--------------------------------------------------------------------
//
// display
//
void display(void)
{
    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, black);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
//--------------------------------------------------------------------
//
// main
//
int
main(int argc, char** argv)
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(640, 480, "Triangles", NULL,
                                          NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();
    init();
    while (!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}



*/











