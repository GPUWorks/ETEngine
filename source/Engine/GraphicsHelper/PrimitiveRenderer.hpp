#pragma once
#include "../StaticDependancies/glad/glad.h"

#include <typeinfo>

//Abstract intetrface
class PrimitiveGeometry
{
public:
	PrimitiveGeometry() {}
	virtual ~PrimitiveGeometry() {}

	virtual const std::type_info& GetType() const = 0;
	void RootDraw();
protected:
	virtual void Draw() = 0;
	virtual void Initialize() = 0;

	bool m_IsInitialized = false;
};

class PrimitiveRenderer : public Singleton<PrimitiveRenderer>
{
public:
	PrimitiveRenderer();
	virtual ~PrimitiveRenderer();

	template<class T>
	void Draw()
	{
		const std::type_info& ti = typeid(T);
		for (PrimitiveGeometry* geometry : m_pTypes)
		{
			const std::type_info& gType = geometry->GetType();
			if (gType == ti)
			{
				geometry->RootDraw();
			}
		}
	}

	void AddGeometry(PrimitiveGeometry* pGeometry);

private:

	std::vector<PrimitiveGeometry*> m_pTypes;
};

//Specifications
namespace primitives
{
	class Quad : public PrimitiveGeometry
	{
	public:
		Quad() {}
		virtual ~Quad();
		const std::type_info& GetType() const { return typeid(Quad); }
	protected:
		void Draw(); //vec3 pos, vec2 tc
		void Initialize();
	private:
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
	};

	class Cube : public PrimitiveGeometry
	{
	public:
		Cube() {}
		virtual ~Cube();
		const std::type_info& GetType() const { return typeid(Cube); }
	protected:
		void Draw(); //vec3 pos, vec3 norm, vec2 tc
		void Initialize();
	private:
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
	};

	template<int32 level>
	class IcoSphere : public PrimitiveGeometry
	{
	public:
		IcoSphere<level>() {}
		virtual ~IcoSphere();
		const std::type_info& GetType() const { return typeid(IcoSphere<level>); }
	protected:
		void Draw(); //vec3 pos
		void Initialize();
	private:
		// #todo: generate with index buffer
		void SubAndPush(std::vector<glm::vec3> &vertices, const int32 lev, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) const;
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
		int32 m_NumVerts;
	};
}