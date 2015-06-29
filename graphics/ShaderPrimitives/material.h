#ifndef MATERIAL_H
#define MATERIAL_H

#include "sampler.h"
#include "texture.h"
#include "kdgeometryshader.h"

#include <QMap>
#include <QOpenGLShaderProgram>
#include <QPair>
#include <QSharedPointer>

class QOpenGLFunctions_3_1;
class QOpenGLFunctions_1_3;

class KDGeometryShader;

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

class TextureUnitConfiguration : public QPair<TexturePtr, SamplerPtr>
{
public:
    TextureUnitConfiguration()
        : QPair<TexturePtr, SamplerPtr>( TexturePtr(), SamplerPtr() )
    {
    }

    explicit TextureUnitConfiguration( const TexturePtr& texture, const SamplerPtr& sampler )
        : QPair<TexturePtr, SamplerPtr>( texture, sampler )
    {
    }

    void setTexture( const TexturePtr& texture ) { first = texture; }
    TexturePtr texture() const { return first; }

    void setSampler( const SamplerPtr sampler ) { second = sampler; }
    SamplerPtr sampler() const { return second; }
};

class Material
{
public:
    Material();
    ~Material();

    void bind();

    void setShaders( const QString& vertexShader,
                     const QString& fragmentShader );
#if !defined(QT_OPENGL_ES_2)
    void setShaders( const QString& vertexShader,
                     const QString& geometryShader,
                     const QString& fragmentShader );

#if !defined(Q_OS_MAC)
    void setShaders( const QString& vertexShader,
                     const QString& geometryShader,
                     const QString& tessellationControlShader,
                     const QString& tessellationEvaluationShader,
                     const QString& fragmentShader );
#endif
#endif

    void setShader( const QOpenGLShaderProgramPtr& shader );

    QOpenGLShaderProgramPtr shader() const { return m_shader; }

    void setTextureUnitConfiguration( GLuint unit, TexturePtr texture, SamplerPtr sampler );
    void setTextureUnitConfiguration( GLuint unit, TexturePtr texture, SamplerPtr sampler, const QByteArray& uniformName );

    void setTextureUnitConfiguration( GLuint unit, TexturePtr texture );
    void setTextureUnitConfiguration( GLuint unit, TexturePtr texture, const QByteArray& uniformName );

    TextureUnitConfiguration textureUnitConfiguration( GLuint unit ) const;

private:
    void initialize();
    bool isInitialized() const { return m_funcsType != Unknown; }

    // For now we assume that we own the shader
    /** \todo Allow this to use reference to non-owned shader */
    QOpenGLShaderProgramPtr m_shader;

    // TODO Qt 5.1 Remove KDGeometryShader and use QOpenGLShaderProgram's support
    // for geometry shaders
#if !defined(QT_OPENGL_ES_2)
    KDGeometryShader* createExtraShaderStage( KDGeometryShader::ShaderStage stage,
                                              const QString& shaderFileName );

    KDGeometryShader* m_geoShader;
    KDGeometryShader* m_tessControl;
    KDGeometryShader* m_tessEval;
#endif

    // This map contains the configuration for the texture units
    QMap<GLuint, TextureUnitConfiguration> m_unitConfigs;
    QMap<GLuint, QByteArray> m_samplerUniforms;

    union
    {
        QOpenGLFunctions_3_1* core;
        QOpenGLFunctions_1_3* compat;
    } m_funcs;

    enum
    {
        Unknown,
        Core,
        Compatibility
    } m_funcsType;
};

typedef QSharedPointer<Material> MaterialPtr;

#endif // MATERIAL_H
