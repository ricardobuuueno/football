#pragma once

#include "publisher_base.hpp"

namespace pub
{

class gazeta_esportiva : public pub::publisher_base
{
  public:
    gazeta_esportiva() : pub::publisher_base()
    {
    }

    [[nodiscard]] auto get_championship(const std::string &page_url) -> std::string override
    {
        return {};
    }

    [[nodiscard]] auto get_championship_clubs(const std::string &page_url) -> std::list<std::string> override
    {
        std::string page_content = extract_html_page(page_url);

        GumboOutput *parsed_response = gumbo_parse(page_content.c_str());

        _clubs.clear();

        locate_clubs(parsed_response->root, _clubs);

        gumbo_destroy_output(&kGumboDefaultOptions, parsed_response);

        return _clubs;
    }

  private:
    std::list<std::string> _clubs{};

    auto locate_clubs(GumboNode *node, std::list<std::string> &result) -> void
    {
        if (node->type != GUMBO_NODE_ELEMENT)
            return;

        if (node->v.element.tag == GUMBO_TAG_TD)
        {
            GumboAttribute *attr = gumbo_get_attribute(&node->v.element.attributes, "class");
            std::string attr_value{attr->value};
            if (attr_value == "table__team")
            {
                for (int i{0}; i < node->v.element.children.length; ++i)
                {
                    if (GumboNode *child = static_cast<GumboNode *>(node->v.element.children.data[i]); child != nullptr)
                    {
                        if (child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_A)
                        {
                            if (GumboAttribute *attr2 = gumbo_get_attribute(&child->v.element.attributes, "class");
                                attr2 != nullptr)
                            {
                                std::string attr_value{attr2->value};
                                if (attr_value == "team-link")
                                {
                                    GumboNode *text = static_cast<GumboNode *>(child->v.element.children.data[0]);
                                    result.push_back(text->v.text.text);
                                }
                            }
                        }
                    }
                }
            }
            return;
        }

        GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; i++)
            locate_clubs(static_cast<GumboNode *>(children->data[i]), result);
    }
};

} // namespace pub