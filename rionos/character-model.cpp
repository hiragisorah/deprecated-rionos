#include "character-model.h"
#include "camera.h"

void CharacterModel::Initialize(void)
{
	this->player_ = std::make_shared<Renderer>();
	this->player_->constant_buffer_ = &this->cbuffer_;
	this->player_->draw_mode_ = DRAW_MODE_BACK_BUFFER_3D;
	this->player_->model_ = this->model_;
	this->player_->shader_ = ShaderPath::shader_3d;

	this->entity().lock()->scene().lock()->graphics()->AddRenderer(this->player_);
}

void CharacterModel::Finalize(void)
{
}

void CharacterModel::Pause(void)
{
}

void CharacterModel::Update(void)
{
}

void CharacterModel::Always(void)
{
	auto & camera = this->entity().lock()->scene().lock()->GetSystem<Camera>();
	auto & transform = this->entity().lock()->GetComponent<Transform>();

	this->cbuffer_.world_ = transform.lock()->GetTransformMatrix();
	this->cbuffer_.view_ = camera.lock()->view();
	this->cbuffer_.projection_ = camera.lock()->projection();
}