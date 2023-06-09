#include "glwidget.h"

#include "backend.h"

void GLWidget::scaleModel(float scaleFactor) {
  scaleModelC(_cubeVertices, _n_vertices, scaleFactor);
  update();
}
/*!
 * \brief GLWidget::moveModel
 *
 * Translates the model by the given x, y, and z offsets. \n
 * This function updates the vertex positions in _cubeVertices and calls
 * update() to trigger a repaint of the widget. \n
 *
 * \param x The x-axis offset.
 * \param y The y-axis offset.
 * \param z The z-axis offset.
 */
void GLWidget::moveModel(float x, float y, float z) {
  moveModelC(_cubeVertices, _n_vertices, x, y, z);
  update();
}
/*!
 * \brief GLWidget::rotateModel
 *
 * Rotates the model by the given x, y, and z angles in degrees. This function
 * updates the vertex positions in _cubeVertices using a rotation matrix and
 * calls update() to trigger a repaint of the widget.
 *
 * \param xAngle The rotation angle in degrees around the x-axis.
 * \param yAngle The rotation angle in degrees around the y-axis.
 * \param zAngle The rotation angle in degrees around the z-axis.
 */
void GLWidget::rotateModel(float xAngle, float yAngle, float zAngle) {
  const float xAngleRadian = xAngle / 360.0f * 2.0f * M_PI;
  const float yAngleRadian = yAngle / 360.0f * 2.0f * M_PI;
  const float zAngleRadian = zAngle / 360.0f * 2.0f * M_PI;

  for (int i = 0; i < _n_vertices * 3; i += 3) {
    const float x = _cubeVertices[i];
    const float y = _cubeVertices[i + 1];
    const float z = _cubeVertices[i + 2];

    float x_rezult = 0.0f;
    float y_rezult = 0.0f;
    float z_rezult = 0.0f;

    rotateX(xAngleRadian, x, y, z, &x_rezult, &y_rezult, &z_rezult);

    _cubeVertices[i] = x_rezult;
    _cubeVertices[i + 1] = y_rezult;
    _cubeVertices[i + 2] = z_rezult;
  }

  for (int i = 0; i < _n_vertices * 3; i += 3) {
    const float x = _cubeVertices[i];
    const float y = _cubeVertices[i + 1];
    const float z = _cubeVertices[i + 2];

    float x_rezult = 0.0f;
    float y_rezult = 0.0f;
    float z_rezult = 0.0f;

    rotateY(yAngleRadian, x, y, z, &x_rezult, &y_rezult, &z_rezult);

    _cubeVertices[i] = x_rezult;
    _cubeVertices[i + 1] = y_rezult;
    _cubeVertices[i + 2] = z_rezult;
  }

  for (int i = 0; i < _n_vertices * 3; i += 3) {
    const float x = _cubeVertices[i];
    const float y = _cubeVertices[i + 1];
    const float z = _cubeVertices[i + 2];

    float x_rezult = 0.0f;
    float y_rezult = 0.0f;
    float z_rezult = 0.0f;

    rotateZ(zAngleRadian, x, y, z, &x_rezult, &y_rezult, &z_rezult);

    _cubeVertices[i] = x_rezult;
    _cubeVertices[i + 1] = y_rezult;
    _cubeVertices[i + 2] = z_rezult;
  }

  update();
}
/*!
 * \brief GLWidget::GLWidget
 *
 * Constructor for the GLWidget class. Initializes the OpenGL widget, parses the
 * input OBJ file, and emits a modelLoaded signal. It also initializes the
 * initial colors, point size, and loads the settings.
 *
 * \param parent The parent QWidget for this GLWidget.
 */

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      _n_vertices(0),
      _n_indices(0),
      _cubeVertices(nullptr),
      _cubeIndices(nullptr) {
  // Make sure the widget has a valid OpenGL context
  setFormat(QSurfaceFormat::defaultFormat());
  parseObjFile(
      "/home/finchren/school/s21_3DViewer/s21_3DViewer/src/3D_Viewer/models/"
      "cube_first.obj",
      &_cubeVertices, &_n_vertices, &_cubeIndices, &_n_indices);

  emit modelLoaded(_n_vertices, _n_indices / 2);
  // The initial color to black
  backgroundColor = QColor(0, 0, 0);
  // The initial point size
  vertexSize = 20.0f;
  // Set the initial vertex color to blue
  vertexColor = QColor(0, 128, 255);
  // Set the initial edge color to white
  edgeColor = QColor(255, 255, 255);
  loadSettings();
}
/*!
 * \brief GLWidget::initializeGL
 *
 * Initializes OpenGL functions, enables vertex arrays for drawing, specifies
 * the vertex data format and location in the array, sets line stipple for
 * dashed lines, and sets the initial edge color.
 */
void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  // Enable the use of vertex arrays for drawing
  glEnableClientState(GL_VERTEX_ARRAY);
  // Specify the format and the location of the vertex data in the array
  glVertexPointer(3, GL_FLOAT, 0, _cubeVertices);
  // Enable line stipple for dashed lines
  if (!isDashedEdges) {
    glLineStipple(1, 0xFFFF);
  } else {
    glLineStipple(1, 0x00FF);
  }
  glLineWidth(edgeThickness);
  // Set the default edge color to white
  glColor3f(1.0f, 1.0f, 1.0f);
}
/*!
 * \brief GLWidget::paintGL
 *
 * Renders the 3D model by drawing vertices and edges with the specified colors
 * and styles. It also sets up the projection and model-view matrices, updates
 * the vertex and index pointers, and enables/disables line stipple based on the
 * isDashedEdges flag.
 */
void GLWidget::paintGL() {
  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
  // Set background color: RGB and opacity
  glClearColor(backgroundColor.redF(), backgroundColor.greenF(),
               backgroundColor.blueF(), backgroundColor.alphaF());

  // Enable line stipple for dashed lines
  if (isDashedEdges) {
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  // Clear the color buffer (color values of the pixels displayed on the screen)
  // and depth buffer (distance of each pixel)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadMatrixf(projectionMatrix.constData());

  // QMatrix4x4 is a data type that represents a 4x4 matrix, used for
  // transformations in 3D graphics Set up the model-view matrix
  QMatrix4x4 modelView;
  // First set of values - position of the camera
  // Second set of values - position of the center
  // Third set of values - determines the orientation of the camera properly (in
  // relation to the ground and other objects in the scene)
  modelView.lookAt(QVector3D(2, 2, 4), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

  // Load the projection and model-view matrices
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projectionMatrix.constData());
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(modelView.constData());

  // Change point size
  glPointSize(vertexSize);

  // Update the vertex and index pointers every time paintGL is called
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _cubeVertices);

  // Draw the points
  glColor3f(vertexColor.redF(), vertexColor.greenF(), vertexColor.blueF());

  if (vertexDisplayMethod == Circle) {
    // Draw the vertices as circles
    for (int i = 0; i < _n_vertices * 3; i += 3) {
      glBegin(GL_TRIANGLE_FAN);
      float x = _cubeVertices[i];
      float y = _cubeVertices[i + 1];
      float z = _cubeVertices[i + 2];
      glVertex3f(x, y, z);
      for (int angle = 0; angle <= 360; angle += 10) {
        float rad = M_PI * angle / 180.0f;
        glVertex3f(x + vertexSize * cos(rad) / width(),
                   y + vertexSize * sin(rad) / height(), z);
      }
      glEnd();
    }
  } else if (vertexDisplayMethod == Square) {
    // Draw the vertices as squares
    for (int i = 0; i < _n_vertices * 3; i += 3) {
      glBegin(GL_QUADS);
      float x = _cubeVertices[i];
      float y = _cubeVertices[i + 1];
      float z = _cubeVertices[i + 2];
      float halfSize = vertexSize / 2.0f;
      glVertex3f(x - halfSize / width(), y - halfSize / height(), z);
      glVertex3f(x + halfSize / width(), y - halfSize / height(), z);
      glVertex3f(x + halfSize / width(), y + halfSize / height(), z);
      glVertex3f(x - halfSize / width(), y + halfSize / height(), z);
      glEnd();
    }
  }

  // Draw the lines
  glColor3f(edgeColor.redF(), edgeColor.greenF(), edgeColor.blueF());
  glDrawElements(GL_LINES, _n_indices, GL_UNSIGNED_INT, _cubeIndices);
}
/*!
 * \brief GLWidget::resizeGL
 *
 * Updates the viewport, projection matrix, and calls update() when the widget
 * is resized.
 *
 * \param width The new width of the widget.
 * \param height The new height of the widget.
 */
void GLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);

  // setToIdentity resets the projection matrix
  projectionMatrix.setToIdentity();
  if (isParallelProjection) {
    projectionMatrix.ortho(-1.0, 1.0, -1.0, 1.0, 0.1, 100.0);
  } else {
    projectionMatrix.perspective(50.0, (double)width / height, 0.1, 100.0);
  }
  update();
}
/*!
 * \brief GLWidget::~GLWidget
 *
 * Destructor class \n
 * Saves the display settings of edges, vertices and BG color. \n
 * After the settings are saved, frees the arrays of vertices and indicies
 */
GLWidget::~GLWidget() {
  saveSettings();
  if (_cubeVertices) {
    free(_cubeVertices);
  }
  if (_cubeIndices) {
    free(_cubeIndices);
  }
}
/*!
 * \brief GLWidget::loadModel
 *
 * The function is used to open the file picker, so u user can choose an obj
 * file to be loaded. \param fileName QFileDialog::getOpenFileName is used to
 * get the filename of QString type and pass it to this function.
 */
void GLWidget::loadModel(const QString& fileName) {
  QByteArray byteArray = fileName.toLocal8Bit();
  const char* filePath = byteArray.constData();

  if (_cubeVertices) {
    free(_cubeVertices);
    _cubeVertices = NULL;
  }
  if (_cubeIndices) {
    free(_cubeIndices);
    _cubeIndices = NULL;
  }

  // Display the filename in the QLabel
  if (filenameLabel) {
    QFileInfo fileInfo(fileName);
    filenameLabel->setText(fileInfo.fileName());
  }

  // Reset _n_vertices and _n_indices before parsing the new file
  _n_vertices = 0;
  _n_indices = 0;

  parseObjFile(filePath, &_cubeVertices, &_n_vertices, &_cubeIndices,
               &_n_indices);

  emit modelLoaded(_n_vertices, _n_indices / 2);
  update();
}
/*!
 * \brief GLWidget::setParallelProjection
 *
 * The function is used to change the type of the projection for the model.
 * \param updateValue
 * Parameter is used to change the value of isParallelProjection boolean
 * variable.\n That variable is saved in case the user closes the app and is
 * loaded when the app starts again.
 */
void GLWidget::setParallelProjection(bool updateValue) {
  projectionMatrix.setToIdentity();
  // Define the orthogonal (parallel) projection matrix's clipping planes:
  //    -1.0: left plane
  //    1.0: right plane
  //    -1.0: bottom plane
  //    1.0: top plane
  //    0.1: near plane (closest distance to the camera where objects are still
  //    visible) 100.0: far plane (farthest distance from the camera where
  //    objects are still visible)
  projectionMatrix.ortho(-1.0, 1.0, -1.0, 1.0, 0.1, 100.0);
  if (updateValue) {
    isParallelProjection = true;
  }
  update();
}
/*!
 * \brief GLWidget::setCentralProjection
 *
 * The function is used to change the type of the projection for the model.
 * \param updateValue
 * Parameter is used to change the value of isParallelProjection boolean
 * variable.\n That variable is saved in case the user closes the app and is
 * loaded when the app starts again.
 */
void GLWidget::setCentralProjection(bool updateValue) {
  projectionMatrix.setToIdentity();
  // First value - field of view angle in degrees. Determines how much of the
  // scene is visible Second value - aspect ratio of the screen. Ensures that
  // the scene is displayed properly on screens of different sizes and aspect
  // ratios Third value - distance from the viewer to the near clipping plane,
  // which determines how close objects can be to the viewer before they are
  // clipped (not rendered) Forth value - distance from the viewer to the far
  // clipping plane, which determines how far objects can be from the viewer
  // before they are clipped Clipping is necessary because the graphics pipeline
  // can only render objects that are within a certain range of distances from
  // the viewer, and any objects that fall outside of this range are clipped or
  // removed from the scene before rendering.
  projectionMatrix.perspective(50.0, (double)width() / height(), 0.1, 100.0);
  if (updateValue) {
    isParallelProjection = false;
  }
  update();
}
/*!
 * \brief GLWidget::setEdgeLineStyle
 *
 * The functio is used to set the style of edges.\n
 * By default the style is set to solid
 * \param style
 * Should take 0x00FF (dashed) or 0xFFFF (solid).\n
 *  0x00FF is a hexadecimal representation of a 16-bit binary number.\n
 *  In this case, it represents the pattern 0000 0000 1111 1111 in binary,\n
 *  which is used for line stippling. When you use 0x00FF as the pattern,\n
 *  it alternates between one visible pixel and one hidden pixel, creating a
 * dashed line effect. \param updateValue Is used to decide if we want to update
 * the the boolean value of isDashedEdges, which is stored in case the user
 * closes the app
 */
void GLWidget::setEdgeLineStyle(unsigned int style, bool updateValue) {
  makeCurrent();
  glLineStipple(1, style);
  if (updateValue) {
    if (style == 0x00FF) {
      isDashedEdges = true;
    } else {
      isDashedEdges = false;
    }
  }
  update();
}
/*!
 * \brief GLWidget::setEdgeWidth
 *
 * The function is used to update the width of the edges.
 * \param widthIncrement
 * It takes widthIncrement argument of float type, which increments
 * edgeThickness value. \n The value cannot become less than 1.0. It is reset
 * back to 1.0 if it becomes less.
 */
void GLWidget::setEdgeWidth(float widthIncrement) {
  makeCurrent();
  edgeThickness += widthIncrement;
  if (edgeThickness < 1.0f) {
    edgeThickness = 1.0f;
  }
  glLineWidth(edgeThickness);
  update();
}
/*!
 * \brief GLWidget::setEdgeColor
 *
 * The function is used to update the color of the edges.
 * \param color
 * QColorDialog::getColor is used to get the color of QColor type and pass it to
 * the function.
 */
void GLWidget::setEdgeColor(const QColor& color) {
  edgeColor = color;
  update();
}
/*!
 * \brief GLWidget::changeVertexSize
 *
 * The function is used to change the vertex size by passing a certain argument
 * that is going tobe used to update the vertexSize variable. \param increment
 * The parameter is of float type. Value is incremented from the initial value.
 * The vertexSize cannot become less than 1.0f.\n It resets to 1.0 when becomes
 * less than 1.0f
 */
void GLWidget::changeVertexSize(float increment) {
  vertexSize += increment;
  if (vertexSize < 1.0f) {
    vertexSize = 1.0f;
  }
  update();
}
/*!
 * \brief GLWidget::setVertexColor
 *
 * The function is used to change the default color of the vertices.
 * \param color The function takes the parameter color of QColor type. \n
 * In the click on button even QColorDialog::getColor is used to get the new
 * color.
 */
void GLWidget::setVertexColor(const QColor& color) {
  vertexColor = color;
  update();
}
/*!
 * \brief GLWidget::setVertexDisplayMethod
 *
 * The functon is used to change the vertex display method between 3 enum values
 * - None, Circle and Square \param method Takes one of the 3 enum values -
 * None, Circle and Square that is going to use to update the view of the model.
 */
void GLWidget::setVertexDisplayMethod(VertexDisplayMethod method) {
  vertexDisplayMethod = method;
  update();
}
/*!
 * \brief GLWidget::saveSettings
 *
 * The function is used to save the settings of the vertex preferences and
 * settings of the edges \n As well as the background that the user has chosen
 * using QSettings.
 */
void GLWidget::saveSettings() {
  QSettings settings("finchren", "3D_Viewer");
  settings.setValue("backgroundColor", backgroundColor);
  settings.setValue("scaleFactor", scaleFactor);
  settings.setValue("vertexSize", vertexSize);
  settings.setValue("vertexColor", vertexColor);
  settings.setValue("edgeColor", edgeColor);
  settings.setValue("vertexDisplayMethod", vertexDisplayMethod);
  settings.setValue("isParallelProjection", isParallelProjection);
  settings.setValue("isDashedEdges", isDashedEdges);
  settings.setValue("edgeThickness", edgeThickness);
}
/*!
 * \brief GLWidget::loadSettings
 *
 * The function is used to load the settings of the vertex preferences and
 * settings of the edges \n As well as the background that the user has chosen
 * using QSettings. If no value was found a default one is used.
 * @params The function uses the global values and takes no arguments as it's
 * parameters.
 */
void GLWidget::loadSettings() {
  QSettings settings("finchren", "3D_Viewer");

  if (settings.contains("backgroundColor")) {
    backgroundColor = settings.value("backgroundColor").value<QColor>();
  } else {
    backgroundColor = QColor(0, 0, 0);
  }

  if (settings.contains("scaleFactor")) {
    scaleFactor = settings.value("scaleFactor").toFloat();
  } else {
    scaleFactor = 1.0f;
  }

  if (settings.contains("vertexSize")) {
    vertexSize = settings.value("vertexSize").toFloat();
  } else {
    vertexSize = 3.0f;
  }

  if (settings.contains("vertexColor")) {
    vertexColor = settings.value("vertexColor").value<QColor>();
  } else {
    vertexColor = QColor(255, 255, 255);
  }

  if (settings.contains("edgeColor")) {
    edgeColor = settings.value("edgeColor").value<QColor>();
  } else {
    edgeColor = QColor(255, 255, 255);
  }

  if (settings.contains("vertexDisplayMethod")) {
    vertexDisplayMethod = static_cast<VertexDisplayMethod>(
        settings.value("vertexDisplayMethod").toInt());
  } else {
    vertexDisplayMethod = Circle;
  }
  if (settings.contains("isParallelProjection")) {
    isParallelProjection = settings.value("isParallelProjection").toBool();
  } else {
    isParallelProjection = false;
  }

  if (settings.contains("isDashedEdges")) {
    isDashedEdges = settings.value("isDashedEdges").toBool();
  } else {
    isDashedEdges = false;
  }

  if (settings.contains("edgeThickness")) {
    edgeThickness = settings.value("edgeThickness").toFloat();
  } else {
    edgeThickness = 1.0f;
  }
}
/*!
 * \brief GLWidget::takeScreenshot
 *
 * \return Returns grabFramebuffer which renders and returns a 32-bit RGB image
 * of the framebuffer.
 */
QImage GLWidget::takeScreenshot() { return grabFramebuffer(); }
/*!
 * \brief GLWidget::resetPreferences
 *
 * The functon is used when the "Reset model's preferences" button is clicked \n
 * It sets BG color to black, edges to solid, thickness of the edges to 1.0f \n
 * Color of the edges to white, vertices display type to circles, size of
 * vertices to 20.0f and the color of verices to blue
 * @param The function takes no parameters and updates the global ones.
 */
void GLWidget::resetPreferences() {
  // The initial BG color to black
  backgroundColor = QColor(0, 0, 0);
  // Type of projection to central
  setCentralProjection(true);
  // Edges to solid
  isDashedEdges = false;
  // Edges thickness to 1.0
  edgeThickness = 1.0f;
  setEdgeWidth(1.0f);
  // Set the edge color to white
  edgeColor = QColor(255, 255, 255);
  // Vertices display method to circles
  setVertexDisplayMethod(GLWidget::Circle);
  // The initial point size to 20.0f
  vertexSize = 20.0f;
  // Set the vertex color to blue
  vertexColor = QColor(0, 128, 255);
  update();
}
