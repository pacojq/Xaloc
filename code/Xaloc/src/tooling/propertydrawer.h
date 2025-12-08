#pragma once
#include "xaloc/types.h"
#include "xaloc/core/core.h"
#include "xaloc/core/guid.h"
#include "xaloc/core/assets/asset.h"
#include "tooling/imgui/core.h"
#include <glm/glm.hpp>

namespace Xaloc {
	class ZEntity;
	class ZScene;
}


namespace Xaloc
{

/// <summary>
/// Used to draw vector properties.
/// </summary>
struct PropertyDrawerCoord
{
	ImVec4 color;
	ImVec4 colorHovered;
	ImVec4 colorActive;

	const char sLabel[2];
	const char sLabelId[4];
	const char sValueId[5];
};
	
// TODO: turn this into a non-static class
//       that way we can keep track of state, like disabled scopes

class ZPropertyDrawer
{
public:
	ZPropertyDrawer::ZPropertyDrawer(const char* sName);
	ZPropertyDrawer::~ZPropertyDrawer();

	void Begin();
	void End();

	void OpenDisabledScope(bool bDisabled = true);
	void CloseDisabledScope();

	void Guid(const char* sLabel, ZGuid nGuid, bool bError = false);

	bool Asset(const char* sLabel, const EAssetType eType, ZAssetHandle& value, void* icon = nullptr);

	bool String(const char* sLabel, std::string& value, bool error = false);

	bool Int(const char* sLabel, int& value, int delta = 1);
	bool Int(const char* sLabel, int& value, int delta, int min);
	bool Int(const char* sLabel, int& value, int delta, int min, int max);

	bool Float(const char* sLabel, float& value, float delta = 0.1f);
	bool Float(const char* sLabel, float& value, float delta, float min);
	bool Float(const char* sLabel, float& value, float delta, float min, float max);

	bool Vec2(const char* sLabel, glm::vec2& value, float delta = 0.1f);
	bool Vec2(const char* sLabel, glm::vec2& value, float delta, float min);
	bool Vec2(const char* sLabel, glm::vec2& value, float delta, float min, float max);

	bool Vec3(const char* sLabel, glm::vec3& value, float delta = 0.1f);
	bool Vec4(const char* sLabel, glm::vec4& value, float delta = 0.1f);

	bool ComboBox(
		int32& rSelectedIndex,
		const char* sLabel,
		const std::vector<std::string>& aValues,
		const std::string& sCurrentValue);

private:
	uint32 m_nId;
	bool m_bOpen;
};

}