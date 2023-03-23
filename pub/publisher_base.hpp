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
    virtual auto get_championship(const std::string &page_url) -> std::tuple<bool, std::string, std::string> = 0;
    virtual auto get_championship_clubs(const std::string &page_url)
        -> std::tuple<bool, std::string, std::list<std::string>> = 0;

  protected:
    publisher_base()
    {
    }

    //"https://www.gazetaesportiva.com/campeonatos/"
    std::string extract_html_page(const std::string &page)
    {
        cpr::Url url = cpr::Url{page + "/"};
        cpr::Response response = cpr::Get(url);
        return response.text;
    }

  private:
};

} // namespace pub