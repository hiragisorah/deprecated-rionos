#include "displacement-field.h"
#include "camera.h"

void DisplacementField::Initialize(void)
{
	auto & camera = this->entity().lock()->scene().lock()->GetSystem<Camera>();

	this->field_ = std::make_shared<Renderer>();

	this->field_->blend_state_ = BLEND_STATE_OPAQUE;
	this->field_->draw_mode_ = DRAW_MODE_BACK_BUFFER_DISP;
	this->field_->rasterizer_state_ = RASTERIZER_STATE_CCW;
	this->field_->sampler_state_ = SAMPLER_STATE_POINT_WRAP;
	this->field_->shader_ = ShaderPath::tesselation;
	this->field_->texture_2d_ = { TexturePath::Simple, TexturePath::Simple_normal };
	this->field_->constant_buffer_ = &this->cbuffer_;

	this->cbuffer_.world_ = DirectX::XMMatrixIdentity();
	this->cbuffer_.view_ = camera.lock()->view();
	this->cbuffer_.projection_ = camera.lock()->projection();

	this->entity().lock()->scene().lock()->graphics()->AddRenderer(this->field_);
}

void DisplacementField::Finalize(void)
{
}

void DisplacementField::Pause(void)
{
}

void DisplacementField::Update(void)
{
}

void DisplacementField::Always(void)
{
}
