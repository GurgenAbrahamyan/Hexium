#pragma once

#pragma once
#include <string>

struct TagComponent {
    std::string tag;

    TagComponent() = default;
    explicit TagComponent(const std::string& tag) : tag(tag) {}
    explicit TagComponent(std::string&& tag) : tag(std::move(tag)) {}
};