#include "AssetManager.h"

#include "AudioManager.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "ShaderType.h"

void AssetManager::LoadShaders()
{
	ResourceManager::Instance().CreateShaderProgram("sprite",
	                                                Shader(Vertex,
	                                                       arkanoid::ASSETS_OFFSET +
	                                                       "res/shaders/sprite/shader.vert"),
	                                                Shader(Fragment,
	                                                       arkanoid::ASSETS_OFFSET +
	                                                       "res/shaders/sprite/shader.frag"));

	ResourceManager::Instance().CreateShaderProgram("text",
	                                                Shader(Vertex,
	                                                       arkanoid::ASSETS_OFFSET +
	                                                       "res/shaders/text/shader.vert"),
	                                                Shader(Fragment,
	                                                       arkanoid::ASSETS_OFFSET +
	                                                       "res/shaders/text/shader.frag"));

	ResourceManager::Instance().CreateShaderProgram("particle",
	                                                Shader(Vertex,
	                                                       arkanoid::ASSETS_OFFSET +
	                                                       "res/shaders/particle/shader.vert"),
	                                                Shader(Fragment,
	                                                       arkanoid::ASSETS_OFFSET +
	                                                       "res/shaders/particle/shader.frag"));

	ResourceManager::Instance().CreateShaderProgram("postprocessing",
	                                                Shader(Vertex,
	                                                       arkanoid::ASSETS_OFFSET +
	                                                       "res/shaders/postprocessing/shader.vert"),
	                                                Shader(Fragment,
	                                                       arkanoid::ASSETS_OFFSET +
	                                                       "res/shaders/postprocessing/shader.frag"));
}

void AssetManager::LoadTextures()
{
	ResourceManager::Instance().CreateTexture("background",
	                                          arkanoid::ASSETS_OFFSET + "res/textures/background.jpg",
	                                          1600, 900);
	ResourceManager::Instance().CreateTexture("face",
	                                          arkanoid::ASSETS_OFFSET + "res/textures/awesome_face.png",
	                                          512, 512, 4, GL_RGBA);
	ResourceManager::Instance().CreateTexture("block",
	                                          arkanoid::ASSETS_OFFSET + "res/textures/block.png",
	                                          128, 128);
	ResourceManager::Instance().CreateTexture("block_solid",
	                                          arkanoid::ASSETS_OFFSET + "res/textures/block_solid.png",
	                                          128, 128);
	ResourceManager::Instance().CreateTexture("paddle",
	                                          arkanoid::ASSETS_OFFSET + "res/textures/paddle.png",
	                                          512, 128, 4, GL_RGBA);
	ResourceManager::Instance().CreateTexture("particle",
	                                          arkanoid::ASSETS_OFFSET + "res/textures/particle.png",
	                                          500, 500, 4, GL_RGBA);
	ResourceManager::Instance().CreateTexture("speedUp",
	                                          arkanoid::ASSETS_OFFSET +
	                                          "res/textures/powerups/powerup_speedup.png",
	                                          512, 128, 4, GL_RGBA);
	ResourceManager::Instance().CreateTexture("sticky",
	                                          arkanoid::ASSETS_OFFSET +
	                                          "res/textures/powerups/powerup_sticky.png",
	                                          512, 128, 4, GL_RGBA);
	ResourceManager::Instance().CreateTexture("passThrough",
	                                          arkanoid::ASSETS_OFFSET +
	                                          "res/textures/powerups/powerup_passthrough.png",
	                                          512, 128, 4, GL_RGBA);
	ResourceManager::Instance().CreateTexture("padSizeIncrease",
	                                          arkanoid::ASSETS_OFFSET +
	                                          "res/textures/powerups/powerup_increase.png",
	                                          512, 128, 4, GL_RGBA);
	ResourceManager::Instance().CreateTexture("confuse",
	                                          arkanoid::ASSETS_OFFSET +
	                                          "res/textures/powerups/powerup_confuse.png",
	                                          512, 128, 4, GL_RGBA);
	ResourceManager::Instance().CreateTexture("chaos",
	                                          arkanoid::ASSETS_OFFSET + "res/textures/powerups/powerup_chaos.png",
	                                          512, 128, 4, GL_RGBA);
}

void AssetManager::LoadAudioFiles()
{
	AudioManager::Instance().CreateAudioSource("background", arkanoid::ASSETS_OFFSET + "res/audio/background.ogg",
	                                           true);
	AudioManager::Instance().CreateAudioSource("bleep", arkanoid::ASSETS_OFFSET + "res/audio/bleep.ogg", false);
	AudioManager::Instance().CreateAudioSource("bleepPaddle",
	                                           arkanoid::ASSETS_OFFSET + "res/audio/bleep_paddle.ogg", false);
	AudioManager::Instance().CreateAudioSource("solid", arkanoid::ASSETS_OFFSET + "res/audio/solid.ogg", false);
	AudioManager::Instance().CreateAudioSource("powerup", arkanoid::ASSETS_OFFSET + "res/audio/powerup.ogg",
	                                           false);
}
