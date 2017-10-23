#pragma once
#include "AbstractComponent.hpp"

class TransformComponent : public AbstractComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	void Translate(float x, float y, float z);
	void Translate(const vec3& position);
	void SetPosition(float x, float y, float z);
	void SetPosition(const vec3& position);

	//void RotateEuler(float x, float y, float z);
	//void RotateEuler(const vec3& rotation);
	void Rotate(const quat& rotation);
	void SetRotation(const quat& rotation) { m_Rotation = rotation; }

	void Scale(float x, float y, float z);
	void Scale(const vec3& scale);

	const vec3& GetPosition() const { return m_Position; }
	const vec3& GetWorldPosition() const { return m_WorldPosition; }
	const vec3& GetScale() const { return m_Scale; }
	const vec3& GetWorldScale() const { return m_WorldScale; }
	const quat& GetRotation() const { return m_Rotation; }
	//const vec3& GetYawPitchRoll() const { return vec3(yaw(m_Rotation), pitch(m_Rotation), roll(m_Rotation)); };
	const quat& GetWorldRotation() const { return m_WorldRotation; }
	const mat4& GetWorld() const { return m_World; }

	const vec3& GetForward() const { return m_Forward; }
	const vec3& GetUp() const { return m_Up; }
	const vec3& GetRight() const { return m_Right; }

protected:

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void DrawForward();


	void UpdateTransforms();

private:
	enum TransformChanged {
		NONE = 0x00,
		TRANSLATION = 0x01,
		ROTATION = 0x02,
		SCALE = 0x04,
	};

	uint8 m_IsTransformChanged;

	vec3 m_Position = vec3(0, 0, 0),
		m_WorldPosition = vec3(0, 0, 0),
		m_Scale = vec3(1, 1, 1),
		m_WorldScale = vec3(1, 1, 1),
		m_Forward = vec3(0, 0, 1),
		m_Up = vec3(0, 1, 0),
		m_Right = vec3(1, 0, 0);
	quat m_Rotation = quat(0, 0, 0, 1),
		m_WorldRotation = quat(0, 0, 0, 1);
	mat4 m_World;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TransformComponent(const TransformComponent& obj);
	TransformComponent& operator=(const TransformComponent& obj) { delete this; }
};

