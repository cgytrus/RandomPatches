#include <Geode/Geode.hpp>

using namespace geode::prelude;

// this entire file is full of hacks, please do not try to comprehend what is going on here

const GLchar* vertPosColorInst = R"(
attribute vec4 a_position;
uniform vec4 u_color;
uniform float u_pointSize;
uniform vec2 u_offset;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
#else
varying vec4 v_fragmentColor;
#endif

void main() {
    gl_Position = CC_MVPMatrix * (a_position + vec4(u_offset.x, u_offset.y, 0.0, 0.0) * gl_InstanceID);
    gl_PointSize = u_pointSize;
    v_fragmentColor = u_color;
}
)";

const GLchar* fragPosColorInst = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;

void main() {
    gl_FragColor = v_fragmentColor;
}
)";

CCGLProgram* s_shader = nullptr;
GLint s_colorLoc = 0;
GLint s_offsetLoc = 0;
bool s_isDrawing = false;

// get address from one of the ccDrawColor* funcs
ccColor4F& s_color = *reinterpret_cast<ccColor4F*>(base::getCocos() + 0x19fb60);
//ccColor4F s_color;
//static void ccDrawColor4FHook(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
//    ccDrawColor4F(r, g, b, a);
//    s_color = { r, g, b, a };
//}
//static void ccDrawColor4BHook(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
//    ccDrawColor4B(r, g, b, a);
//    s_color = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
//}

#include <Geode/modify/DrawGridLayer.hpp>
class $modify(DrawGridLayer) {
    $override void draw() {
        s_isDrawing = true;
        DrawGridLayer::draw();
        s_isDrawing = false;
    }
};

static void ccDrawFreeHook() {
    ccDrawFree();
    if (s_shader)
        s_shader->release();
}

static void ccDrawLinesHook(const CCPoint* lines, unsigned int numberOfLines) {
    if (!s_isDrawing) {
        ccDrawLines(lines, numberOfLines);
        return;
    }
    s_isDrawing = false;

    if (numberOfLines < 8) {
        ccDrawLines(lines, numberOfLines);
        return;
    }

    if (!s_shader) {
        s_shader = new CCGLProgram();
        s_shader->initWithVertexShaderByteArray(vertPosColorInst, fragPosColorInst);
        s_shader->addAttribute("aVertex", kCCVertexAttrib_Position);
        s_shader->link();
        s_shader->updateUniforms();
        s_colorLoc = s_shader->getUniformLocationForName("u_color");
        s_offsetLoc = s_shader->getUniformLocationForName("u_offset");
    }

    float height = lines[1].y - lines[0].y;
    float width = lines[7].x - lines[6].x;
    float offX = std::min(lines[2].x - lines[0].x, lines[4].x - lines[2].x);
    float offY = std::min(lines[8].y - lines[6].y, lines[10].y - lines[8].y);
    GLsizei horCount = std::ceil(width / offX);
    GLsizei verCount = std::ceil(height / offY);

    GLsizei horSplitIndex = std::ceil((0.0f - lines[0].x) / offX);
    GLsizei verSplitIndex = std::ceil((90.0f - lines[6].y) / offY);

    GLsizei horCountA = std::min(horCount, horSplitIndex);
    GLsizei horCountB = horCount - horCountA;
    GLsizei verCountA = std::min(verCount, verSplitIndex);
    GLsizei verCountB = verCount - verCountA;

    ccVertex2F horVert[4] = {
        { lines[0].x, lines[0].y },
        { lines[1].x, lines[1].y }
    };
    ccVertex2F verVert[4] = {
        { lines[6].x, lines[6].y },
        { lines[7].x, lines[7].y }
    };

    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_colorLoc, &s_color.r, 1);

    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position);

    s_shader->setUniformLocationWith2f(s_offsetLoc, offX, 0.f);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, horVert);
    glDrawArraysInstanced(GL_LINES, 0, 2, horCountA);
    if (horCountB > 1) {
        horVert[0].x += offX * static_cast<float>(horSplitIndex + 1);
        horVert[1].x += offX * static_cast<float>(horSplitIndex + 1);
        glDrawArraysInstanced(GL_LINES, 0, 2, horCountB - 1);
    }

    s_shader->setUniformLocationWith2f(s_offsetLoc, 0.f, offY);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, verVert);
    glDrawArraysInstanced(GL_LINES, 0, 2, verCountA);
    if (verCountB > 1) {
        verVert[0].y += offY * static_cast<float>(verSplitIndex + 1);
        verVert[1].y += offY * static_cast<float>(verSplitIndex + 1);
        glDrawArraysInstanced(GL_LINES, 0, 2, verCountB - 1);
    }

    CC_INCREMENT_GL_DRAWS(2);
}

#include "patch.hpp"
#include <sinaps.hpp>

$execute {
    if (!Mod::get()->getSettingValue<bool>("optimized-grid"))
        return;

    //LOG_ERR(Mod::get()->hook(
    //    reinterpret_cast<void*>(getNonVirtual(&ccDrawColor4F)),
    //    &ccDrawColor4FHook,
    //    "cocos2d::ccDrawColor4F",
    //    tulip::hook::TulipConvention::Cdecl
    //));
    //LOG_ERR(Mod::get()->hook(
    //    reinterpret_cast<void*>(getNonVirtual(Resolve<GLubyte, GLubyte, GLubyte, GLubyte>::func(&ccDrawColor4B))),
    //    &ccDrawColor4BHook,
    //    "cocos2d::ccDrawColor4B",
    //    tulip::hook::TulipConvention::Cdecl
    //));

    LOG_ERR(Mod::get()->hook(
        reinterpret_cast<void*>(getNonVirtual(&ccDrawFree)),
        &ccDrawFreeHook,
        "cocos2d::ccDrawFree",
        tulip::hook::TulipConvention::Cdecl
    ));

    LOG_ERR(Mod::get()->hook(
        reinterpret_cast<void*>(getNonVirtual(&ccDrawLines)),
        &ccDrawLinesHook,
        "cocos2d::ccDrawLines",
        tulip::hook::TulipConvention::Cdecl
    ));

    // change max drawn lines to 6 per coord
    // idk why its specifically those numbers but thats what me from months ago thought was good
    //auto addr = reinterpret_cast<uint8_t*>(getVirtual(&DrawGridLayer::draw));
    auto addr = reinterpret_cast<uint8_t*>(base::get() + 0x2db8f0);
    addr += sinaps::find<"90 01">(addr, 0x1000);
    LOG_ERR(Mod::get()->patch(addr, { 0x08, 0x00 }));
    addr += sinaps::find<"90 01">(addr + 2, 0x1000) + 2;
    LOG_ERR(Mod::get()->patch(addr, { 0x0f, 0x00 }));

    //auto drawAddr = base::get() + 0x2db8f0;
    //LOG_ERR(Mod::get()->patch(reinterpret_cast<void*>(drawAddr + 0x3a4), { 0x08, 0x00 }));
    //LOG_ERR(Mod::get()->patch(reinterpret_cast<void*>(drawAddr + 0x508), { 0x0f, 0x00 }));
}
