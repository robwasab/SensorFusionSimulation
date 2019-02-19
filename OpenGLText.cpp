//
//  OpenGLText.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/26/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLText.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLPrimitive.hpp"

#include <assert.h>
#include <ft2build.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

#include FT_FREETYPE_H

#define ARIAL_FILEPATH      "/Library/Fonts/Arial.ttf"
#define VERTEX_SHADER       "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/OpenGLText_VertexShader.fsh"
#define FRAGMENT_SHADER     "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/OpenGLText_FragmentShader.fsh"

struct Character
{
    GLchar      value;
    GLuint      texture_id;     // ID handle of the glyph texture
    glm::ivec2  size;           // Size of glyph
    glm::ivec2  bearing;        // Offset from baseline to left/top of glyph
    FT_Pos      advance;        // Offset to advance to next glyph
};

static std::map<GLchar, Character> gCharacters;
static size_t gCharactersRefCount = 0;
static OpenGLShader * gShader;

OpenGLText * OpenGLText::mOpenGLText = NULL;

static void load_glyphs_into_opengl(void)
{
    assert(gCharacters.size() == 0);
    assert(gShader == NULL);
    
    //printf("Initializing OpenGLText shader\n");
    gShader = new OpenGLShader(VERTEX_SHADER, FRAGMENT_SHADER);
    
    FT_Error result;
    
    FT_Library ft_lib;
    result = FT_Init_FreeType(&ft_lib);
    assert(result == 0);
    
    FT_Face face;
    result = FT_New_Face(ft_lib, ARIAL_FILEPATH, 0, &face);
    assert(result == 0);
    
    FT_UInt font_height_px = 100;
    FT_Set_Pixel_Sizes(face, 0 /*dynamic calc*/, font_height_px);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restr.
    
    //printf("Generating text!\n");
    
    for(GLubyte c = 0; c < 128; c++)
    {
        // load character glyph
        result = FT_Load_Char(face, c, FT_LOAD_RENDER);
        assert(result == 0);
        
        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        
        glTexImage2D(
                     GL_TEXTURE_2D,      // target
                     0,                  // mipmap level
                     GL_RED,             // internal fmt, num color components
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,                  // border
                     GL_RED,             // fmt of pixel data
                     GL_UNSIGNED_BYTE,   // data type of pixel data
                     face->glyph->bitmap.buffer
                     );
        
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        Character character;
        character.value = c;
        character.texture_id = texture_id;
        character.size = glm::ivec2(face->glyph->bitmap.width,
                                    face->glyph->bitmap.rows);
        
        character.bearing = glm::ivec2(face->glyph->bitmap_left,
                                       face->glyph->bitmap_top);
        
        character.advance = face->glyph->advance.x;
        
        //printf("[%c].sizex %d\n", character.value, character.size.x);
        //printf("[%c].sizey %d\n", character.value, character.size.y);
        //printf("[%c].bearx %d\n", character.value, character.bearing.x);
        //printf("[%c].beary %d\n", character.value, character.bearing.y);
        //printf("[%c].advan %ld\n", character.value, character.advance);
        
        gCharacters.insert(std::pair<GLchar, Character>(c, character));
    }
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft_lib);
}

OpenGLText::OpenGLText(float window_size_x, float window_size_y)
{
    if(gCharacters.size() == 0)
    {
        gCharactersRefCount = 1;
        
        // empty so initialize
        load_glyphs_into_opengl();
    }
    else
    {
        gCharactersRefCount++;
    }
    
    mWindowDimX = window_size_x;
    mWindowDimY = window_size_y;
    mProjection = glm::ortho(0.0f, window_size_x, 0.0f, window_size_y);
    
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER,            // target
                 sizeof(GLfloat) * 6 * 4,    // size of buffer
                 NULL,                       // data to copy
                 GL_DYNAMIC_DRAW);           // usage
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                    // vertex attribute to target
                          4,                    // number of components per attr
                          GL_FLOAT,             // data type of each component in attr
                          GL_FALSE,             // normalized?
                          4 * sizeof(GLfloat),  // stride
                          0);                   // offset in element
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    mRgb = OpenGLPrimitive::COLOR_ORANGE;
    mFontHeight = 100.0f;
    mOriginX = 0.0;
    mOriginY = 0.0;
}

OpenGLText * OpenGLText::getOpenGLTextWithWindowDimensions(float window_size_x, float window_size_y)
{
    if(OpenGLText::mOpenGLText == NULL)
    {
        OpenGLText::mOpenGLText = new OpenGLText(window_size_x, window_size_y);
    }
    else
    {
        mOpenGLText->windowResizedTo(window_size_x, window_size_y);
    }
    return mOpenGLText;
}


void OpenGLText::windowResizedTo(float size_x, float size_y)
{
    assert(size_x >= 0);
    assert(size_y >= 0);
    
    mWindowDimX = size_x;
    mWindowDimY = size_y;
    mProjection = glm::ortho(0.0f, size_x, 0.0f, size_y);
}

void OpenGLText::setColor(float * rgb)
{
    assert(rgb != NULL);
    
    mRgb = rgb;
}

void OpenGLText::setFontHeight(float height)
{
    assert(height >= 0.0f);
    
    mFontHeight = height;
}

void OpenGLText::setBottomLeftCornerOrigin(float xorigin, float yorigin)
{
    assert(xorigin >= 0);
    assert(xorigin <= mWindowDimX);
    assert(yorigin >= 0);
    assert(yorigin <= mWindowDimY);
    
    mOriginX = xorigin;
    mOriginY = yorigin;
}

float OpenGLText::calculateStringWidth(const char text[])
{
    size_t text_len = strlen(text);
    
    float scale = mFontHeight / 100.0f;
    float width = 0.0f;
    
    for(size_t k = 0; k < text_len; k++)
    {
        Character c = gCharacters[text[k]];
        
        width += (c.advance / 64) * scale;
    }
    
    return width;
}

void OpenGLText::drawStringWithTransform(const char text[], glm::mat4 transform)
{
    glm::mat4 view_transform = mProjection * transform;
    
    gShader->use();
    gShader->setVec3("texture_color", mRgb[0], mRgb[1], mRgb[2]);
    gShader->setMat4("transform", view_transform);
    gShader->setInt("texture_fontface", 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(mVAO);
    
    float scale = mFontHeight / 100.0f;
    
    size_t text_len = strlen(text);
    
    float x = mOriginX;
    float y = mOriginY;
    
    for(size_t k = 0; k < text_len; k++)
    {
        Character c = gCharacters[text[k]];
        
        GLfloat xpos = x + c.bearing.x * scale;
        GLfloat ypos = y - (c.size.y - c.bearing.y) * scale;
        
        GLfloat w = c.size.x * scale;
        GLfloat h = c.size.y * scale;
        
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        
        //printf("texture id: %d\n", c.texture_id);
        
        glBindTexture(GL_TEXTURE_2D, c.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferSubData(GL_ARRAY_BUFFER,    // target
                        0,                  // offset
                        sizeof(vertices),   // size
                        vertices);
    
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        x += (c.advance / 64) * scale;
    }
}

OpenGLText::~OpenGLText()
{
    gCharactersRefCount--;
    
    if(0 == gCharactersRefCount)
    {
        std::map<GLchar, Character>::iterator iterator;
        
        iterator = gCharacters.begin();
        
        size_t count = 0;
        
        for(; iterator != gCharacters.end(); iterator++)
        {
            std::pair<GLchar, Character> item = *iterator;
            
            printf("Deleting %c...\n", item.first);
            
            glDeleteTextures(1, &item.second.texture_id);
            
            count++;
        }
        printf("Deleted %zu items...\n", count);
    }
    
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
}
