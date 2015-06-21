/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

/**
 * \file graphics/opengl/gldevice.h
 * \brief OpenGL implementation - CGLDevice class
 */

#pragma once

#include "graphics/core/device.h"
#include "graphics/opengl/glutil.h"
#include "graphics/opengl/glframebuffer.h"

#include <string>
#include <vector>
#include <set>
#include <map>


// Graphics module namespace
namespace Gfx {

/**
 \enum VertexBufferType
 \brief Specifies type of vertex buffer to use
 */
enum VertexBufferType
{
   VBT_DISPLAY_LIST,    //! use display lists
   VBT_VBO_CORE,        //! use core OpenGL 1.5 VBOs
   VBT_VBO_ARB          //! use ARB extension VBOs
};

enum ShadowMappingSupport
{
    SMS_NONE,           //! No support for depth textures
    SMS_ARB,            //! ARB extension
    SMS_CORE            //! Core support
};

struct GLDevicePrivate;

/**
  \class CGLDevice
  \brief Implementation of CDevice interface in OpenGL

  Provides the concrete implementation of 3D device in OpenGL.

  This class should be initialized (by calling Initialize() ) only after
  setting the video mode by CApplication, once the OpenGL context is defined.
  Because of that, CGLDeviceConfig is outside the CDevice class and must be set
  in CApplication.
*/
class CGLDevice : public CDevice
{
public:
    CGLDevice(const DeviceConfig &config);
    virtual ~CGLDevice();

    virtual void DebugHook() OVERRIDE;
    virtual void DebugLights() OVERRIDE;

    virtual bool Create() OVERRIDE;
    virtual void Destroy() OVERRIDE;

    virtual void ConfigChanged(const DeviceConfig &newConfig) OVERRIDE;

    virtual void BeginScene() OVERRIDE;
    virtual void EndScene() OVERRIDE;

    virtual void Clear() OVERRIDE;

    virtual void SetTransform(TransformType type, const Math::Matrix &matrix) OVERRIDE;

    virtual void SetMaterial(const Material &material) OVERRIDE;

    virtual int GetMaxLightCount() OVERRIDE;
    virtual void SetLight(int index, const Light &light) OVERRIDE;
    virtual void SetLightEnabled(int index, bool enabled) OVERRIDE;

    virtual Texture CreateTexture(CImage *image, const TextureCreateParams &params) OVERRIDE;
    virtual Texture CreateTexture(ImageData *data, const TextureCreateParams &params) OVERRIDE;
    virtual Texture CreateDepthTexture(int width, int height, int depth) OVERRIDE;
    virtual void DestroyTexture(const Texture &texture) OVERRIDE;
    virtual void DestroyAllTextures() OVERRIDE;

    virtual int GetMaxTextureStageCount() OVERRIDE;
    virtual void SetTexture(int index, const Texture &texture) OVERRIDE;
    virtual void SetTexture(int index, unsigned int textureId) OVERRIDE;
    virtual void SetTextureEnabled(int index, bool enabled) OVERRIDE;

    virtual void SetTextureStageParams(int index, const TextureStageParams &params) OVERRIDE;

    virtual void SetTextureStageWrap(int index, Gfx::TexWrapMode wrapS, Gfx::TexWrapMode wrapT) OVERRIDE;
    virtual void SetTextureCoordGeneration(int index, TextureGenerationParams &params) OVERRIDE;

    virtual void DrawPrimitive(PrimitiveType type, const Vertex *vertices    , int vertexCount,
                               Color color = Color(1.0f, 1.0f, 1.0f, 1.0f)) OVERRIDE;
    virtual void DrawPrimitive(PrimitiveType type, const VertexTex2 *vertices, int vertexCount,
                               Color color = Color(1.0f, 1.0f, 1.0f, 1.0f)) OVERRIDE;
    virtual void DrawPrimitive(PrimitiveType type, const VertexCol *vertices , int vertexCount) OVERRIDE;

    virtual unsigned int CreateStaticBuffer(PrimitiveType primitiveType, const Vertex* vertices, int vertexCount) OVERRIDE;
    virtual unsigned int CreateStaticBuffer(PrimitiveType primitiveType, const VertexTex2* vertices, int vertexCount) OVERRIDE;
    virtual unsigned int CreateStaticBuffer(PrimitiveType primitiveType, const VertexCol* vertices, int vertexCount) OVERRIDE;
    virtual void UpdateStaticBuffer(unsigned int bufferId, PrimitiveType primitiveType, const Vertex* vertices, int vertexCount) OVERRIDE;
    virtual void UpdateStaticBuffer(unsigned int bufferId, PrimitiveType primitiveType, const VertexTex2* vertices, int vertexCount) OVERRIDE;
    virtual void UpdateStaticBuffer(unsigned int bufferId, PrimitiveType primitiveType, const VertexCol* vertices, int vertexCount) OVERRIDE;
    virtual void DrawStaticBuffer(unsigned int bufferId) OVERRIDE;
    virtual void DestroyStaticBuffer(unsigned int bufferId) OVERRIDE;

    virtual int ComputeSphereVisibility(const Math::Vector &center, float radius) OVERRIDE;

    virtual void SetViewport(int x, int y, int width, int height) OVERRIDE;

    virtual void SetRenderState(RenderState state, bool enabled) OVERRIDE;

    virtual void SetColorMask(bool red, bool green, bool blue, bool alpha) OVERRIDE;

    virtual void SetDepthTestFunc(CompFunc func) OVERRIDE;

    virtual void SetDepthBias(float factor, float units) OVERRIDE;

    virtual void SetAlphaTestFunc(CompFunc func, float refValue) OVERRIDE;

    virtual void SetBlendFunc(BlendFunc srcBlend, BlendFunc dstBlend) OVERRIDE;

    virtual void SetClearColor(const Color &color) OVERRIDE;

    virtual void SetGlobalAmbient(const Color &color) OVERRIDE;

    virtual void SetFogParams(FogMode mode, const Color &color, float start, float end, float density) OVERRIDE;

    virtual void SetCullMode(CullMode mode) OVERRIDE;

    virtual void SetShadeModel(ShadeModel model) OVERRIDE;

    virtual void SetShadowColor(float value) OVERRIDE;

    virtual void SetFillMode(FillMode mode) OVERRIDE;

    virtual void CopyFramebufferToTexture(Texture& texture, int xOffset, int yOffset, int x, int y, int width, int height) OVERRIDE;

    virtual void* GetFrameBufferPixels() const OVERRIDE;

    virtual CFramebuffer* GetFramebuffer(std::string name) OVERRIDE;

    virtual CFramebuffer* CreateFramebuffer(std::string name, const FramebufferParams& params) OVERRIDE;

    virtual void DeleteFramebuffer(std::string name) OVERRIDE;

private:
    //! Updates internal modelview matrix
    void UpdateModelviewMatrix();
    //! Updates position for given light based on transformation matrices
    void UpdateLightPosition(int index);
    //! Updates the texture params for given texture stage
    void UpdateTextureParams(int index);

private:
    //! Current config
    DeviceConfig m_config;

    //! Current world matrix
    Math::Matrix m_worldMat;
    //! Current view matrix
    Math::Matrix m_viewMat;
    //! OpenGL modelview matrix = world matrix * view matrix
    Math::Matrix m_modelviewMat;
    //! Current projection matrix
    Math::Matrix m_projectionMat;

    //! The current material
    Material m_material;

    //! Whether lighting is enabled
    bool m_lighting;
    //! Current lights
    std::vector<Light> m_lights;
    //! Current lights enable status
    std::vector<bool> m_lightsEnabled;

    //! Current textures; \c NULL value means unassigned
    std::vector<Texture> m_currentTextures;
    //! Current texture stages enable status
    std::vector<bool> m_texturesEnabled;
    //! Current texture params
    std::vector<TextureStageParams> m_textureStageParams;

    //! Set of all created textures
    std::set<Texture> m_allTextures;

    //! Map of framebuffers
    std::map<std::string, CFramebuffer*> m_framebuffers;

    //! Type of vertex structure
    enum VertexType
    {
        VERTEX_TYPE_NORMAL,
        VERTEX_TYPE_TEX2,
        VERTEX_TYPE_COL,
    };

    //! Info about static VBO buffers
    struct VboObjectInfo
    {
        PrimitiveType primitiveType;
        unsigned int bufferId;
        VertexType vertexType;
        int vertexCount;
    };

    //! Detected capabilities
    //! OpenGL version
    int m_glMajor, m_glMinor;
    //! Depth texture support
    ShadowMappingSupport m_shadowMappingSupport;
    //! Shadow ambient support
    bool m_shadowAmbientSupported;
    //! Whether to use multitexturing
    bool m_multitextureAvailable;
    //! Whether to use VBOs or display lists
    bool m_vboAvailable;
    //! Whether anisotropic filtering is available
    bool m_anisotropyAvailable;
    //! Maximum anisotropy level
    int m_maxAnisotropy;
    //! Whether offscreen rendering is available
    bool m_framebufferObject;
    //! Framebuffer support
    FramebufferSupport m_framebufferSupport;
    //! Which vertex buffer type to use
    VertexBufferType m_vertexBufferType;
    //! Map of saved VBO objects
    std::map<unsigned int, VboObjectInfo> m_vboObjects;
    //! Last ID of VBO object
    unsigned int m_lastVboId;
};


} // namespace Gfx

