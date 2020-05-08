
///////////////////////////////////////////////////////////////////////////
/*
 * as mentioned in previous projects, shaders are little programs that rest
 * on the GPU. These programs are ran for each specific section of the
 * graphics pipeline. In a basic sense, shaders are nothing more than
 * programs transforming inputs to outputs. Shaders are also very isolated
 * programs in that they're not allowed to communicate with each other; the
 * only communication they have is via their inputs and outputs.
 *
 * This program will explain shaders and specifically the OpenGL Shading
 * Language, in a more general fashion.
 */
///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  /*
   * A shader typically has the following structure:
   */

    ///////////////////////////////////////////////////////////////////////////
    /*
     *if(0)
    {
      #version version_number
      in type in_variable_name;
      in type in_variable_name;

      out type out_variable_name;

      uniform type uniform_name;

      void main();
      {
        // process input(s) and do some weird graphics stuff
        ...
        // output processed stuff to output variable
        our_variable_name = weird_stuff_we_processed;
      }
    }
     */
    ///////////////////////////////////////////////////////////////////////////

    /*
     * in a vertex shader, each shader input variable is a vertex attribute.
     * There is a max number of vertex attributes we're allowed to declare
     * limited by the hardware. OpenGL guarantees there are always at least
     * 16 4-component vertex attributes available, but some hardware might
     * allow for more which you can retrieve by querying GL_MAX_VERTEX_ATTRIBS:
     */

  ///////////////////////////////////////////////////////////////////////////
  /*
   * int nrAttributes;
   * glGetIntergerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
   * std::cout << "Maximum nr of vertex attributes supported: " <<
   * nrAttributes << std::endl;
   *
   * This ofter returns the minimum of 16 which should be more than enough
   * for most purposes.
   */
  ///////////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////////
  /*
   * Types
   *  GLSL has data types for specifying what kind of variable we want to
   *  work with. GLSL has some basic types like int, float, double, uint, and
   *  bool. GLSL also features two container types that we'll be using a lot,
   *  namely vectors and matrices. We'll discuss matrices in a later chapter.
   *
   *  a vector in GLSL is a 1, 2, 3, or 4 component container for any of the
   *  basic types. They can take the following form (n represents the number
   *  of components);
   *
   *  vecn: the default vector of n floats.
   *  bvecn: a vector of n booleans.
   *  ivecn: a vector of n integers.
   *  uvecn: a vector of n unsigned integers.
   *  dvecn: a vector of n double components.
   *
   *  Components of a vector can be accessed via vec.x where x is the first
   *  component of the vector. You can use .x, .y, .z and .w to access their
   *  1st, 2nd, 3rd, and 4th component respectively. GLSL also allows you to
   *  use rgba for colors  or stpq.
   *
   *  The vector datatype allows for some intersting and flexible component
   *  selection called swizzling. Swizzling allows us to use syntax like this:
   *
   *  vec2 someVec;
   *  vec4 differentVec = someVec.xyxx;
   *  vec3 anotherVec = differentVec.zyw;
   *  vec4 otherVec = someVec.xxxx + another vec.yxzy;
   *
   *  We can also pass vectors as arguments to different vector constructor
   *  calls, reducing the number of arguments required:
   *
   *  vec2 vect = vec2(0.5, 0.7);
   *  vec4 result = vec4(vect, 0.0, 0.0);
   *  vec4 otherResult = vec4(result.xyz, 1.0);
   */
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  /*
   * Ins and outs
   *  GLSL defined in and out keywords specifically for inputs and outputs on
   *  the individual shader so that we can move stuff around. Wherever
   *  an output variable matches with an input variable of the next shader
   *  stage they're passed along. The vertex and fragment shader differ a bit
   *  though.
   *
   *  The vertex shader differs in its input in that it receives its input
   *  straight from the vertex data. To define how tht e vertex data is
   *  organized we specify the input variables with location metadata so we
   *  can configure the vertex attributes on the CPU. We've seen this in the
   *  previous chapter as layout (location = 0). The vertex shader thus
   *  requires  an extra layout specification for its inputs so we can link
   *  it with the vertex data.
   *
   *  the other exception is that the fragment shader requires a vec4 color
   *  output variable since the fragment shaders needs to generate a final
   *  output color. not doing so will make undefined fragments
   *
   *  So if we want to send data from one shader to the other we'd have to
   *  declare an output in the sending shader and a similar input in the
   *  receiving shader.  When the types and the names are equal on both sides
   *  OpenGL will link  those variables together and then it is possible to
   *  send data between shaders.
   */
  ///////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <glad/glad.h>

#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <vector>
#include <algorithm>

  void processInput(GLFWwindow window[0]);

  static void ErrorCallback(int error, const char* description)
  {
    std::cout << "Error: " << error << ", " << description << std::endl;
  }

  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action
          , int mods)
  {
    scancode;
    mods;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  }

  int main()
  {
    std::vector<GLFWwindow*> window;
    GLuint vertBuffer, vertShader, fragShader, program;
    GLint pvmLocation, vposLocation, vcolLocation;


    if(!glfwInit())
    {
      exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    int quantity = 2;

    for(int count = 0; count < 1; count++)
    {
      window.push_back(glfwCreateWindow(640, 480,
                                        std::string("glfw window" + count).c_str() ,
                                        nullptr,
                                        nullptr));
      glfwSetErrorCallback(ErrorCallback);

      glfwMakeContextCurrent(window[count]);
      gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));


      if(!window[count])
      {
        glfwTerminate();
        exit(EXIT_FAILURE);
      }

      glfwSetKeyCallback(window[count], KeyCallback);

      glfwSwapInterval(1);

    }

    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };


    unsigned int VBO;
    glGenBuffers(1, &VBO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    ///////////////////////////////////////////////////////////////////////////
    /*
     * here we declared a vertex color variable as a vec4 outpu8t that we set
     * in the vertex shader and we declare a similar vertexColor input in the
     * fragment shader. Since they both have the same type and name,
     * vertexColor in the fragment shader is linked to the vertexColor in the
     * vertex shader. Because we set the color to a dark-red color in the
     * vertex shader, the resulting fragments should be dark-red as well. The
     * following image shows that output
     */
    ///////////////////////////////////////////////////////////////////////////
    if(0)
    {

    const char* vertexShaderSource = (
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;"
            "out vec4 vertexColor;"
            "void main()"
            "{"
            "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
            "  vertexColor = vec4(0.5, 0.0, 0.0, 1.0);"
            "}");

    const char* fragSource =
            ("#version 330 core\n"
             "out vec4 FragColor;"
             ""
             "in vec4 vertexColor;"
             "void main()"
             "{"
             "  FragColor = vertexColor;"
             "}");
    }

    ///////////////////////////////////////////////////////////////////////////
    /*
     * Uniforms
     *  Uniforms are another way to pass data from our application on the CPU
     *  to the shaders on the GPU. Uniforms are however slightly different
     *  compared to vertex attributes. uniforms are global. Global, meaning
     *  that a uniform variable is unique per shader program object, and can
     *  be accessed from any shader at any stage in the shader program. Also,
     *  whatever you set the uniform value to, uniforms will keep their
     *  values until they're either reset or updated
     *
     *  To declare a uniform in GLSL we simply add the uniform keyword to a
     *  shader with a type and a name. From that point on we can use the
     *  newly declared uniform in the shader. Let's see if this time we can
     *  set the color of the triangle via uniform:
     *
     *  We declared a uniform vec4 ourColor in the fragment shader and set
     *  the fragment's output color to the content of this uniform value.
     *  Since uniforms are global variables, we can define them in any shader
     *  stage we'd like so no need to go through the vertex shader again to
     *  get something to the fragment shader. We're not using this uniform in
     *  the vertex shader so there's no need to define it there
     *
     *    NOTE: if you declare a uniform that isn't used anywhere in your
     *    GLSL code the compiler will silently remove the variable from the
     *    compiled version which is th ecause for the several frustrating
     *    errors; keep this in mind.
     *
     *    GOTO glUseProgram
     */
    ///////////////////////////////////////////////////////////////////////////


      const char* vertexShaderSource = (
              "#version 330 core\n"
              "layout (location = 0) in vec3 aPos;"
              "void main()"
              "{"
              "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
              "}");

      const char* fragSource =
              ("#version 330 core\n"
               "out vec4 FragColor;"
               "uniform vec4 ourColor;"
               "void main()"
               "{"
               "  FragColor = ourColor;"
               "}");

      ///////////////////////////////////////////////////////////////////////////
      /*
       * The uniform is currently empty.
       * We first need to find the index/location of the uniform attribute in
       * our shader. Once we have the index/location of the uniform we can
       * update its values. Instead of passing a single color to the fragment
       * shader.
       */
      ///////////////////////////////////////////////////////////////////////////



    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
                std::endl;
    }



    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSource, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();


    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog <<
                std::endl;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);


    glBindVertexArray(VAO);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);





    double time = glfwGetTime();
    static bool allWindowsClosed = true;
    //render loop here
    do
    {
      allWindowsClosed = true;

      std::for_each(window.begin(), window.end(), [](GLFWwindow* theWindow)
      {
        if(theWindow == nullptr)
          return;

        if(glfwWindowShouldClose(theWindow))
        {
          glfwSetWindowShouldClose(theWindow, GLFW_TRUE);
        }
        else
        {

          allWindowsClosed = false;
        }
      });


      // input
      // -----


      // render
      // ------
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      ///////////////////////////////////////////////////////////////////////////
      /*
       * First, we retrieve the running time in seconds via glfwGetTime().
       * Then we vary the color in the range of 0.0 - 1.0 by using the sin
       * function and store the result in greenValue.
       *
       * Then we query for the location of the ourColor uniform using
       * glGetUniformLocation. We supply the shader program and the name of
       * the uniform to the query function. If glGetUniformLocation returns
       * -1, it could not find the location. Lastly we can set the uniform
       * value using the glUniform4f function. Note that finding the uniform
       * location does not require you to use the shader program first, but
       * updating a uniform does require you to first use the program because
       * it sets the uniform on the currently active shader program.
       */
      ///////////////////////////////////////////////////////////////////////////

      float timeValue = glfwGetTime();
      float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
      int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
      if(vertexColorLocation > -1)
      {
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
      }
      else
      {
        glUseProgram(shaderProgram);
      }

      /////////////////////////////////////////////////////////////////////////
      /*
       * Now that we know how to set the values of uniform variables we can
       * use them for rendering. If we want the color to gradually change, we
       * want to update this uniform every frame, otherwise the triangle
       * would maintain a single solid color if we only set it once. So we
       * calculate the greenValue and update the uniform each render iteration.
       *
       * uniforms are useful tool for setting attributes that might change
       * every frame, or for interchanging data between your application and
       * your shaders, but what if we want to set a color for each vertex? in
       * that case we'd have to declare as many uniforms as we have vertices.
       */
      /////////////////////////////////////////////////////////////////////////

      glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      glDrawArrays(GL_TRIANGLES, 0, 3);
      // glBindVertexArray(0); // no need to unbind it every time

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window[0]);
      glfwPollEvents();


    }while(!allWindowsClosed);


    std::for_each(window.begin(), window.end(), glfwDestroyWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);


  }

