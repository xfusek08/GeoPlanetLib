
#include <algorithm>
#include <stdexcept>

#include <GeoPlanetLib/SurfaceGenerator.h>

using namespace std;
using namespace gp;

SurfaceGenerator::SurfaceGenerator(const std::vector<std::string>& modifierConfig)
{
    for (string ident : modifierConfig) {
        addModifier(ident);
    }
}

ModifierListItem SurfaceGenerator::getModifier(string ident)
{
	auto it = findModifierByIdent(ident);
	if (it == modifierList.end()) {
		return {};
	}
	return *it;
}

ModifierListItem SurfaceGenerator::addModifier(string ident, std::shared_ptr<SurfaceModifier> modifier, bool enabled)
{
    auto itemRef = getModifierReference(ident);
    ModifierListItem newItem = { ident, enabled, modifier };
    if (itemRef == nullptr) {
        modifierList.push_back(newItem);
    } else {
        *itemRef = newItem;
    }
    return newItem;
}

ModifierListItem SurfaceGenerator::addModifier(string ident, string modifierClass, bool enabled)
{
    shared_ptr<SurfaceModifier> modifier = SurfaceModifier::create(modifierClass);
    if (modifier == nullptr) {
        throw invalid_argument("\"" + modifierClass + "\" is not valid modifier class.");
    }
    return addModifier(ident, modifier, enabled);
}

bool SurfaceGenerator::removeModifier(string ident)
{
    auto it = findModifierByIdent(ident);
    if (it != modifierList.end()) {
        modifierList.erase(it);
		return true;
    }
	return false;
}

bool SurfaceGenerator::disableModifier(string ident)
{
    auto item = getModifierReference(ident);
    if (item == nullptr) {
        return false;
    }
    item->enabled = false;
    return true;
}

bool SurfaceGenerator::enableModifier(string ident)
{
    auto item = getModifierReference(ident);
    if (item == nullptr) {
        return false;
    }
    item->enabled = true;
    return true;
}

bool SurfaceGenerator::applyModifiers(shared_ptr<Surface> surface)
{
    for (auto modifierItem : modifierList) {
        if (modifierItem.enabled) {
            if (!modifierItem.modifier->apply(surface)) {
                return false;
            }
        }
    }
    return true;
}

shared_ptr<Surface> SurfaceGenerator::generate(unsigned int resolution)
{
    auto surface = (resolution > 0)
        ? make_shared<Surface>(resolution)
        : make_shared<Surface>();

    applyModifiers(surface);
    return surface;
}

// private

ModifierList::iterator SurfaceGenerator::findModifierByIdent(std::string ident)
{
    return find_if(modifierList.begin(), modifierList.end(),
        [ident](const ModifierListItem& item) {
            return item.ident == ident;
        }
    );
}

ModifierListItem* SurfaceGenerator::getModifierReference(string ident)
{
    auto it = findModifierByIdent(ident);
    if (it == modifierList.end()) {
        return nullptr;
    }
    return &(*it);
}
