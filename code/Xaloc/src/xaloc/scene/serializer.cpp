#include "xapch.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/core/assets/util/parsing.h"
#include "xaloc/core/assets/util/xml.h"
#include "xaloc/scene/serializer.h"
#include "xaloc/scene/scene.h"
#include "xaloc/scene/components.h"
#include "xaloc/debugging/assert.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Xaloc
{

ZRef<ZScene> ZSceneSerializer::Deserialize(pugi::xml_document& doc)
{
	pugi::xml_node root = doc.child("scene");

	std::string strId = root.attribute("uuid").value();
	uint64_t id = ParseUint64(strId);
	std::string name = root.child("name").child_value();

	XA_CORE_TRACE("Loading scene. Id = {0} ({1}). Name = '{2}'", id, strId, name);

	ZRef<ZScene> scene = CreateRef<ZScene>(name, id);

	pugi::xml_node entitiesRoot = root.child("entities");

	//for (pugi::xml_node entity = entitiesRoot.child("entity"); entity; entity = entity.next_sibling("entity"))
	//{
	//	DeserializeEntity(entity, scene);
	//}

	for (pugi::xml_node entity = entitiesRoot.last_child(); entity; entity = entity.previous_sibling())
	{
		DeserializeEntity(entity, scene);
	}

	return scene;
}

void ZSceneSerializer::DeserializeEntity(pugi::xml_node& entityNode, const ZRef<ZScene>& scene)
{
	const std::string sId = entityNode.attribute("uuid").value();
	const uint64_t nId = ParseUint64(sId);
	XA_CORE_TRACE("	Loading entity. Id = {}  ({})", nId, sId);

	ZEntity entity = scene->CreateEntity("New Entity", nId);

	pugi::xml_node tagNode = entityNode.child("Tag");
	if (tagNode)
	{
		std::string tag = tagNode.attribute("tag").value();
		entity.GetComponent<CmpTag>().sTag = tag;
		XA_CORE_TRACE("        Tag: {}", tag);
	}

	pugi::xml_node transformNode = entityNode.child("Transform");
	if (transformNode)
	{
		std::string strPosition = transformNode.attribute("position").value();
		auto vecPosition = ParseVector(strPosition);
		glm::vec3 position = { vecPosition[0], vecPosition[1], vecPosition[2] };

		std::string strRotation = transformNode.attribute("rotation").value();
		auto vecRotation = ParseVector(strRotation);
		glm::vec3 rotation = { vecRotation[0], vecRotation[1], vecRotation[2] };

		std::string strScale = transformNode.attribute("scale").value();
		auto vecScale = ParseVector(strScale);
		glm::vec3 scale = { vecScale[0], vecScale[1], vecScale[2] };

			
		entity.GetComponent<CmpTransform>().Translation = position;
		entity.GetComponent<CmpTransform>().Rotation = rotation;
		entity.GetComponent<CmpTransform>().Scale = scale;

		XA_CORE_TRACE("        Transform: position = {0}	rotation = {1}	scale = {2}",
			strPosition, strRotation, strScale);
	}

	pugi::xml_node behaviourNode = entityNode.child("Behaviour");
	if (behaviourNode)
	{
		std::string name = behaviourNode.attribute("name").value();
		entity.AddComponent<CmpBehaviour>(name);

		XA_CORE_TRACE("        Behaviour: {}", name);
	}

	CmpSpriteRenderer serializedSpriteRenderer;
	if (XmlDeserializeSpriteRenderer(
		serializedSpriteRenderer,
		entityNode))
	{
		CmpSpriteRenderer& spr = entity.AddComponent<CmpSpriteRenderer>();

		spr.Sprite = serializedSpriteRenderer.Sprite;
		spr.Color = serializedSpriteRenderer.Color;
		spr.fTilingFactor = serializedSpriteRenderer.fTilingFactor;
	}
	/*
	pugi::xml_node rendererNode = entityNode.child("CmpSpriteRenderer");
	if (rendererNode)
	{
		std::string strTexcoords = rendererNode.attribute("texcoords").value();
		auto texcoords = ParseVector(strTexcoords);

		uint32_t width = rendererNode.attribute("width").as_uint();
		uint32_t height = rendererNode.attribute("height").as_uint();

		std::string assetID = rendererNode.attribute("assetID").value();
			
		ZRef<Texture2D> tilemap = AssetManager::GetTexture(assetID);
		ZRef<SubTexture2D> subTex = SubTexture2D::CreateFromAbsCoords(tilemap, 
			{ texcoords[0] * tilemap->GetWidth(), texcoords[1] * tilemap->GetHeight() }, { width, height });

		entity.AddComponent<CmpSpriteRenderer>(subTex);

		XA_CORE_TRACE("		CmpSpriteRenderer: texcoords = {0}	width = {1}	height = {2}	assetID = {3}",
			strTexcoords, width, height, assetID);
	}
	*/
}

pugi::xml_document ZSceneSerializer::Serialize(const ZRef<ZScene>& scene)
{
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_string(
		"<!-- Xaloc Engine. Serialized Scene -->",
		pugi::parse_default | pugi::parse_comments);

	if (!res && res.status != pugi::status_no_document_element)
	{
		XA_CORE_ERROR("Parsing status: {}", (int)res.status);
		XA_CORE_ERROR("Parsing error: {}", res.description());
		XA_CORE_ASSERT(false, "Something went wrong!");
	}

	pugi::xml_node root = doc.append_child("scene");

	auto rootId = root.append_attribute("uuid");
	rootId.set_value(Uint64ToString(scene->m_SceneID).c_str());

	pugi::xml_node sceneName = root.append_child("name");
	sceneName.append_child(pugi::node_pcdata).set_value(scene->m_Name.c_str());

	pugi::xml_node entities = root.append_child("entities");

	// Only serialize entities with ID
	auto view = scene->m_Registry.view<CmpId>();
	for (auto entity : view)
	{
		const CmpId& id = view.get<CmpId>(entity);
		SerializeEntity(doc, entities, ZEntity(entity, scene.get()), id.idValue, scene);
	}

	return doc;
}

void ZSceneSerializer::SerializeEntity(
	pugi::xml_document& doc,
	pugi::xml_node& entitiesRoot,
	ZEntity entity,
	ZGuid id,
	const ZRef<ZScene>& scene)
{
	pugi::xml_node node = entitiesRoot.append_child("entity");

	auto attrId = node.append_attribute("uuid");
	attrId.set_value(Uint64ToString(id).c_str());

	if (entity.HasComponent<CmpTag>())
	{
		auto tag = entity.GetComponent<CmpTag>();
		auto tagNode = node.append_child("Tag");
		tagNode.append_attribute("tag").set_value(tag.sTag.c_str());
	}

	if (entity.HasComponent<CmpTransform>())
	{
		auto transform = entity.GetComponent<CmpTransform>();
		auto transformNode = node.append_child("Transform");

		char buff[64];
			
		snprintf(buff, sizeof(buff), "[%f, %f, %f]",transform.Translation.x, transform.Translation.y, transform.Translation.z);
		std::string strPosition = buff;
		snprintf(buff, sizeof(buff), "[%f, %f, %f]", transform.Rotation.x, transform.Rotation.y, transform.Rotation.z);
		std::string strRotation = buff;
		snprintf(buff, sizeof(buff), "[%f, %f, %f]", transform.Scale.x, transform.Scale.y, transform.Scale.z);
		std::string strScale = buff;

		transformNode.append_attribute("position").set_value(strPosition.c_str());
		transformNode.append_attribute("rotation").set_value(strRotation.c_str());
		transformNode.append_attribute("scale").set_value(strScale.c_str());
	}

	if (entity.HasComponent<CmpBehaviour>())
	{
		auto behaviour = entity.GetComponent<CmpBehaviour>();
		auto behaviourNode = node.append_child("Behaviour");
		behaviourNode.append_attribute("name").set_value(behaviour.sModuleName.c_str());
	}

	if (entity.HasComponent<CmpSpriteRenderer>())
	{
		auto renderer = entity.GetComponent<CmpSpriteRenderer>();
		auto rendererNode = node.append_child("SpriteRenderer");
		/*
		auto subTex = renderer.Sprite;
		auto tex = subTex->GetTexture();
			
		char buff[64];
		snprintf(buff, sizeof(buff), "[%f, %f]", subTex->GetTexCoords()->x, subTex->GetTexCoords()->y);
		std::string strTexcoords = buff;

		rendererNode.append_attribute("texcoords").set_value(strTexcoords.c_str());
		rendererNode.append_attribute("width").set_value(subTex->GetWidth());
		rendererNode.append_attribute("height").set_value(subTex->GetHeight());
		*/
		auto attrId = rendererNode.append_attribute("sprite");
		attrId.set_value(Uint64ToString(renderer.Sprite).c_str());
	}
}

}