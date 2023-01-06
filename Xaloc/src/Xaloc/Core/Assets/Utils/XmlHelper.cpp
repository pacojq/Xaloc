#include "xapch.h"
#include "XmlHelper.h"

#include "Xaloc/Core/Assets/AssetManager.h"

#include <fstream>
#include <iomanip>
#include <iostream>

namespace Xaloc {


    static std::string Uint64ToString(uint64_t n)
    {
        char buff[16 + 1];
        snprintf(buff, sizeof(buff), "%llx", n);
        std::string result = buff;
        return result;
    }

    std::vector<float> XmlHelper::ParseVector(const std::string& str)
    {
        std::vector<float> output;
        std::string s = std::string(str);
        char separator = ',';

        s.erase(0, 1); // Remove first "["
        s.erase(str.length() - 1, 1); // Remove last "]"

        std::string::size_type prev_pos = 0, pos = 0;

        while ((pos = s.find(separator, pos)) != std::string::npos)
        {
            std::string substring(s.substr(prev_pos, pos - prev_pos));

            output.push_back(std::stof(substring));

            prev_pos = ++pos;
        }

        output.push_back(std::stof(s.substr(prev_pos, pos - prev_pos))); // Last word

        return output;
    }

    uint64_t XmlHelper::ParseUint64(const std::string& str)
    {
        errno = 0;
        unsigned long long result = strtoull(str.c_str(), NULL, 16);

        if (errno == EINVAL)
        {
            XA_CORE_ASSERT(false, "Not a valid number!");
            return 0;
        }
        else if (errno == ERANGE)
        {
            XA_CORE_ASSERT(false, "Does not fit in an unsigned long long!");
            return 0;
        }

        return result;
    }



    bool XmlHelper::OpenXmlAsset(const AssetMetadata& metadata, pugi::xml_document& doc)
    {
        std::ifstream stream(AssetManager::GetFileSystemPathString(metadata));
        pugi::xml_parse_result docResult = doc.load(stream);

        XA_CORE_TRACE("Asset read file result: {}", docResult.description());
        if (docResult.status != pugi::xml_parse_status::status_ok)
        {
            if (docResult.status == pugi::xml_parse_status::status_file_not_found)
            {
                XA_CORE_ASSERT(false, "Could not load XML. File not found.");
            }
            else XA_CORE_ASSERT(false, "Could not load XML.");
            
            return false;
        }

        return true;
    }


	bool XmlHelper::TryDeserializeCompTransform(pugi::xml_node& parentNode, TransformComponent& outTransform)
	{
        pugi::xml_node transformNode = parentNode.child("TransformComponent");
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

            outTransform = TransformComponent();
            outTransform.Translation = position;
            outTransform.Rotation = rotation;
            outTransform.Scale = scale;

            //XA_CORE_TRACE("        TransformComponent: position = {0}    rotation = {1}    scale = {2}",
            //    strPosition, strRotation, strScale);
        
            return true;
        }
        return false;
	}


    bool XmlHelper::TryDeserializeCompSpriteRenderer(pugi::xml_node& parentNode, SpriteRendererComponent& outDrawing)
    {
        pugi::xml_node rendererNode = parentNode.child("SpriteRendererComponent");
        if (rendererNode)
        {
            //std::string strTexcoords = rendererNode.attribute("texcoords").value();
            //auto texcoords = ParseVector(strTexcoords);

            //uint32_t width = rendererNode.attribute("width").as_uint();
            //uint32_t height = rendererNode.attribute("height").as_uint();

            std::string strSpriteAssetID = rendererNode.attribute("sprite").value();
            uint64_t spriteAssetID = (UUID)ParseUint64(strSpriteAssetID);

            if (AssetManager::GetMetadata(spriteAssetID).IsValid())
            {
                outDrawing = SpriteRendererComponent();
                outDrawing.Sprite = spriteAssetID;
            }
            else
            {
                XA_CORE_ERROR("Sprite renderer could not load sprite with asset ID '{0}', spriteAssetID");

                // TODO load with error texture/material

                outDrawing = SpriteRendererComponent();
                outDrawing.Sprite = 0;
            }

            //XA_CORE_TRACE("        SpriteRendererComponent: assetID = {0}", spriteAssetID);

            return true;
        }
        return false;
    }

}