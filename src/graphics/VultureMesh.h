#pragma once

#include "Vulcro.h"
#include "util/VultureUtil.h"
#include "models/VulturePrimitives.h"

class IMesh : public OOP::Interface {

public:

	virtual VulkanUniformSetRef getUniformSet() {
		return nullptr;
	}

	virtual void draw(vk::CommandBuffer *cmd, uint32 numInstances) = 0;

};

typedef shared_ptr<IMesh> MeshRef;

class ClassicMesh : public IMesh {

public:

	static shared_ptr<ClassicMesh> makeFromPrimitive(VulkanContextRef ctx, Primitives::IPrimitive prim) {
		auto cmr = make_shared<ClassicMesh>();

		vboRef vbuf = ctx->makeVBO<vec4>({
			vk::Format::eR32G32B32A32Sfloat
			}, static_cast<uint32>(prim.vertices.size()), &prim.vertices[0]);

		cmr->setBuffers(
			vbuf,
			ctx->makeIBO(move(prim.indices))
		);
	}

	void setBuffers(vboRef vbuf, iboRef ibuf) {
		_ibo = ibuf;
		_vbo = vbuf;
	}

	virtual void draw(vk::CommandBuffer *cmd, uint32 numInstances) override {

		_vbo->bind(cmd);
		_ibo->bind(cmd);

		if (_ibo != nullptr)
			cmd->drawIndexed(_ibo->getCount(), numInstances, 0, 0, 0);
		else
			cmd->draw(_vbo->getCount(), numInstances, 0, 0);

	};

protected:
	vboRef _vbo;
	iboRef _ibo;
};

typedef shared_ptr<ClassicMesh> ClassicMeshRef;

class BufferlessMesh : public IMesh {
public:

	static shared_ptr<BufferlessMesh> make(uint32 vertexCount) {
		return make_shared<BufferlessMesh>(vertexCount);
	}

	BufferlessMesh(uint32 vertexCount) :
		_vertexCount(vertexCount) 
	{

	}
	
	virtual void draw(vk::CommandBuffer *cmd, uint32 numInstances) override {
		cmd->draw(_vertexCount, numInstances, 0, 0);
	}

protected:
	uint32 _vertexCount;
};

typedef shared_ptr<BufferlessMesh> BufferlessMeshRef;