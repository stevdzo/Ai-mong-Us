#include "StaticGameObject.h"
#include "SOIL2.h"

StaticGameObject::StaticGameObject(bool _has_texture) : BaseGameEntity(0, Vector2D(0, 0), 0) {

    textures = new GLuint[1];
    current_frame = 0;
    number_of_frames = 0;
    transparent = true;
    visible = false;
    flip_horizontal = false;

    size.width = 471 / 14;
    size.height = 717 / 14;

    has_texture = _has_texture;

    position = Vector2D(500, 500);    
}


void StaticGameObject::update(double dt) {

    
}

void StaticGameObject::render() {

    if (has_texture) {

        glEnable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glDisable(GL_DEPTH_TEST);

        if (visible) {
            if (transparent) {

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            glBindTexture(GL_TEXTURE_2D, textures[current_frame]);

            glBegin(GL_QUADS);

            if (flip_horizontal) {

                glTexCoord2f(1, 0);  glVertex2f(position.x - size.width / 2, position.y - size.height / 2);
                glTexCoord2f(0, 0);  glVertex2f(position.x + size.width / 2, position.y - size.height / 2);
                glTexCoord2f(0, 1);  glVertex2f(position.x + size.width / 2, position.y + size.height / 2);
                glTexCoord2f(1, 1);  glVertex2f(position.x - size.width / 2, position.y + size.height / 2);

            }
            else {

                glTexCoord2f(0, 0);  glVertex2f(position.x - size.width / 2, position.y - size.height / 2);
                glTexCoord2f(1, 0);  glVertex2f(position.x + size.width / 2, position.y - size.height / 2);
                glTexCoord2f(1, 1);  glVertex2f(position.x + size.width / 2, position.y + size.height / 2);
                glTexCoord2f(0, 1);  glVertex2f(position.x - size.width / 2, position.y + size.height / 2);
            }
            glEnd();

            if (transparent) { glDisable(GL_BLEND); }
        }
        glDisable(GL_TEXTURE_2D);
    }
    else {

        glColor3f(0.3, 0.7, 0.1);
        glPointSize(6.0f);
        glBegin(GL_POINTS);      
            glVertex2f(position.x, position.y);
        glEnd();
        glColor3f(1, 1, 1);
    }
}

bool StaticGameObject::add_texture(const char* file_name, const bool _transparent) {

    GLuint texture = SOIL_load_OGL_texture(file_name, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    if (texture == 0) {
        return false;
    }

    textures[texture_index] = texture;
    texture_index++;

    transparent = _transparent;

    return true;
}