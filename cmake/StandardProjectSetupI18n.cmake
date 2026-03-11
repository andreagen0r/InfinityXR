include_guard(GLOBAL)

macro(set_standard_project_setup REQUIRED_VERSION)
    message(STATUS "Qt standard project setup with version: ${REQUIRED_VERSION}")

    qt_standard_project_setup(
        REQUIRES ${REQUIRED_VERSION}
        I18N_SOURCE_LANGUAGE
            en-US
        I18N_TRANSLATED_LANGUAGES
            #ISO Code
            #Language-Country   | Language           | Country
            #-------------------|--------------------|-------------------|
            # ar-EG               # Arabic             | Egypt
            # ar-SA               # Arabic             | Saudi Arabia
            # de-DE               # German             | Germany
            # de-AT               # German             | Austria
            # bg-BG               # Bulgarian          | Bulgaria
            # ca-ES               # Catalan            | Spain
            # zh-CN               # Chinese            | China
            # zh-TW               # Chinese            | Taiwan
            # ko-KR               # Korean             | South Korea
            # hr-HR               # Croatian           | Croatia
            # da-DK               # Danish             | Denmark
            # sk-SK               # Slovak             | Slovakia
            # sl-SI               # Slovenian          | Slovenia
            # es-ES               # Spanish            | Spain
            # es-MX               # Spanish            | Mexico
            # et-EE               # Estonian           | Estonia
            # fil-PH              # Filipino           | Philippines
            # fi-FI               # Finnish            | Finland
            # fr-FR               # French             | France
            # fr-CA               # French             | Canada
            # el-GR               # Greek              | Greece
            # he-IL               # Hebrew             | Israel
            # hi-IN               # Hindi              | India
            # nl-NL               # Dutch              | Netherlands
            # nl-BE               # Dutch              | Belgium
            # hu-HU               # Hungarian          | Hungary
            # id-ID               # Indonesian         | Indonesia
            # en-US               # English            | United States
            # en-GB               # English            | United Kingdom
            # is-IS               # Icelandic          | Iceland
            # it-IT               # Italian            | Italy
            # ja-JP               # Japanese           | Japan
            # km-KH               # Khmer              | Cambodia
            # lv-LV               # Latvian            | Latvia
            # lt-LT               # Lithuanian         | Lithuania
            # ms-MY               # Malay              | Malaysia
            # mi-NZ               # Maori              | New Zealand
            # mn-MN               # Mongolian          | Mongolia
            # no-NO               # Norwegian          | Norway
            # fa-IR               # Persian            | Iran
            # pl-PL               # Polish             | Poland
            pt-BR               # Portuguese         | Brazil
            # pt-PT               # Portuguese         | Portugal
            # ro-RO               # Romanian           | Romania
            # ru-RU               # Russian            | Russia
            # sr-RS               # Serbian            | Serbia
            # sv-SE               # Swedish            | Sweden
            # th-TH               # Thai               | Thailand
            # ta-LK               # Tamil              | Sri Lanka
            # cs-CZ               # Czech              | Czech Republic
            # tr-TR               # Turkish            | Turkey
            # uk-UA               # Ukrainian          | Ukraine
            # ur-PK               # Urdu               | Pakistan
            # vi-VN               # Vietnamese         | Vietnam
            # zu-ZA               # Zulu               | South Africa
            # af-ZA               # Afrikaans          | South Africa
            # sq-AL               # Albanian           | Albania
            # am-ET               # Amharic            | Ethiopia
            # hy-AM               # Armenian           | Armenia
            # az-AZ               # Azerbaijani        | Azerbaijan
            # eu-ES               # Basque             | Spain
            # be-BY               # Belarusian         | Belarus
            # my-MM               # Burmese            | Myanmar
            # bs-BA               # Bosnian            | Bosnia and Herzegovina
            # kk-KZ               # Kazakh             | Kazakhstan
            # cy-GB               # Welsh              | United Kingdom
            # ka-GE               # Georgian           | Georgia
            # gn-PY               # Guarani            | Paraguay
            # ha-NG               # Hausa              | Nigeria
            # ga-IE               # Irish              | Ireland
            # lo-LA               # Lao                | Laos
            # lb-LU               # Luxembourgish      | Luxembourg
            # mk-MK               # Macedonian         | North Macedonia
            # ne-NP               # Nepali             | Nepal
            # ps-AF               # Pashto             | Afghanistan
            # ky-KG               # Kyrgyz             | Kyrgyzstan
            # sm-WS               # Samoan             | Samoa
            # ss-SZ               # Swazi              | Eswatini
            # tg-TJ               # Tajik              | Tajikistan
            # tet-TL              # Tetum              | Timor-Leste
            # tn-BW               # Tswana             | Botswana
            # ug-CN               # Uyghur             | China
            # xh-ZA               # Xhosa              | South Africa
)


endmacro()
