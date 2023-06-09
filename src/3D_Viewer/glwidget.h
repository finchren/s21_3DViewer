#ifndef GLWIDGET_H
#define GLWIDGET_H
#define GL_SILENCE_DEPRECATION
#include <QFileInfo>
#include <QLabel>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QSettings>

// For max and min
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cfloat>
#include <cmath>

class GLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT
 public:
  explicit GLWidget(QWidget* parent = nullptr);
  // Destructor declaration for the frees of the arrays
  ~GLWidget();
  void loadModel(const QString& fileName);
  QLabel* filenameLabel;
  QImage takeScreenshot();
  /*!
   * \brief GLWidget::getBackgroundColor
   *
   * Getter function that returns the current background color of the GLWidget.
   *
   * \return The current background color as a QColor object.
   */
  QColor getBackgroundColor() const { return backgroundColor; }
  /*!
   * \brief GLWidget::setBackgroundColor
   *
   * Setter function that sets the background color of the GLWidget to the given
   * QColor object.
   *
   * \param color The QColor object representing the new background color.
   */
  void setBackgroundColor(const QColor& color) { backgroundColor = color; }
  void scaleModel(float scaleFactor);
  void moveModel(float x, float y, float z);
  void rotateModel(float xAngle, float yAngle, float zAngle);
  void setParallelProjection(bool updateValue = true);
  void setCentralProjection(bool updateValue = true);
  void setEdgeLineStyle(unsigned int style, bool updateValue);
  void setEdgeWidth(float widthIncrement);
  void setEdgeColor(const QColor& color);
  void changeVertexSize(float increment);
  void setVertexColor(const QColor& color);
  enum VertexDisplayMethod { None, Circle, Square };
  void setVertexDisplayMethod(VertexDisplayMethod method);
  void saveSettings();
  void loadSettings();
  void resetPreferences();

 public:
  int _n_vertices;
  int _n_indices;
  float* _cubeVertices;
  unsigned int* _cubeIndices;

 signals:
  void modelLoaded(int numVertices, int numEdges);

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

 private:
  float scaleFactor;
  float vertexSize;
  bool isParallelProjection;
  bool isDashedEdges;
  float edgeThickness;
  QColor backgroundColor;
  // Set up the projection matrix
  // QMatrix4x4 is a data type that represents a 4x4 matrix, used for
  // transformations in 3D graphics
  QMatrix4x4 projectionMatrix;
  QColor vertexColor;
  QColor edgeColor;
  VertexDisplayMethod vertexDisplayMethod;
};

#endif  // GLWIDGET_H
