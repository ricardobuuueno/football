#pragma once

#include <list>
#include <string>

#include "gumbo.h"
#include <cpr/cpr.h>

namespace pub
{

enum class publisher : uint8_t
{
    none,
    gazeta_esportiva
};

class publisher_base
{
  public:
    virtual auto get_championship_clubs(const std::string &name) -> std::list<std::string> = 0;

  protected:
    publisher_base(const std::string &base_url) : _base_url(base_url)
    {
    }

    //"https://www.gazetaesportiva.com/campeonatos/"
    std::string extract_html_page(const std::string &page)
    {
        cpr::Url url = cpr::Url{_base_url + page + "/"};
        cpr::Response response = cpr::Get(url);
        return response.text;
    }

  private:
    std::string _base_url;
};

} // namespace pub