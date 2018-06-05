#pragma once

#pragma warning(disable: 4129)

namespace Resource
{
	namespace Model
	{
		constexpr const char * paths[] =
		{
			"Resource/Model/duck.sem",
			"Resource/Model/Flower_hairpin.sem",
			"Resource/Model/Glasses_Red01.sem",
			"Resource/Model/hand.sem",
			"Resource/Model/jeep.sem",
			"Resource/Model/kaoru.sem",
			"Resource/Model/Kaoru_bikini.sem",
			"Resource/Model/Kaoru_Pajama.sem",
			"Resource/Model/Kaoru_schooluniform.sem",
			"Resource/Model/Kunai.sem",
			"Resource/Model/quad2d.sem",
			"Resource/Model/wood.sem",
		};
		enum class PATH
		{
			duck,
			Flower_hairpin,
			Glasses_Red01,
			hand,
			jeep,
			kaoru,
			Kaoru_bikini,
			Kaoru_Pajama,
			Kaoru_schooluniform,
			Kunai,
			quad2d,
			wood,
		};
		namespace FBX
		{
			constexpr const char * paths[] =
			{
				"Resource/Model/FBX/duck.fbx",
				"Resource/Model/FBX/jeep1.fbx",
				"Resource/Model/FBX/kaoru.fbx",
			};
			enum class PATH
			{
				duck,
				jeep1,
				kaoru,
			};
		}
	}
	namespace Shader
	{
		constexpr const char * paths[] =
		{
			"Resource/Shader/shader_2d.hlsl",
			"Resource/Shader/shader_3d.hlsl",
			"Resource/Shader/tesselation.hlsl",
		};
		enum class PATH
		{
			shader_2d,
			shader_3d,
			tesselation,
		};
	}
	namespace Texture
	{
		constexpr const char * paths[] =
		{
			"Resource/Texture/cube.png",
			"Resource/Texture/explosion.png",
			"Resource/Texture/rot.png",
			"Resource/Texture/taisetsu.TIF",
		};
		enum class PATH
		{
			cube,
			explosion,
			rot,
			taisetsu,
		};
	}
}
