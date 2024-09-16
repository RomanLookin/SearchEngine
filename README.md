# Search Engine
## _Implementation of a search system for the corporate portal of the company "ITBox"._

[![N|Solid](https://cldup.com/dTxpPi9lDf.thumb.png)](http://itbox.ru)

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](http://itbox.ru)

Search Engine is a C++ powered search engine.

- builds search indexes
- works with configuration files
- Search results are ranked

## Features

- The search engine is a console application (executable file that runs on any server or computer)
- The search engine automatically crawls all files and indexes them
- settings via JSON format files
- Search results are ranked, sorted and returned to the user.
- Indexing is started every time after update document base.

Поисковый движок представляет из себя консольное приложение, осуществляющее поиск и имеющее возможность настройки поиска через файлы формата JSON. 
Настройка работы приложения осуществляется через конфигурационный файл config.json. В нем задается задаются названия файлов, по которым движок будет осуществлять поиск. Файлы с документами располагаются в папке resource. В файле requests.json задается файл с пользовательскими запросами. В приложении тестирование проводилось с использованием текстов на русском языке. В текстах удалены знаки пунктуации, числительные, цифры, предлоги, слова приведены к нормализованное форме (существительные в единственном числе именительном падеже, глаголы в форме инфинитива). Слова разделяются пробелом. Такая же нормализация проведена для запросов. В файле config первые 4 файла содержат каждый по одному документы (для выполнения требования ТЗ), 5-й файл содержит 996 документов (для проверки варианта загрузки нескольких документов). Конфигурационные файлы должны находится в папке с исполняемым файлом, документы и запросы должны находится в папке resource, которая также должна находится в папке с исполняемым файлом.
В начале выводится меню. По нажатию 1 загружаются документы и происходит расчет индексов слов. Можно подгружать документы после этого (через добавление нового файла в config.json), при этом обновляются индексы слов.  По вводе 2 можно просмотреть загруженные документы. По вводе 3 загружаются запросы. Далее по вводу 4 происходит поиск документов, соответствующих введенному запросу. Результат сохраняется в файле answers.json (вп папке с исполняемым файлом).

## Tech

Приложение разработано в среде Qt creator 4.8.1 на языке С++ (версия 11) в операционной системе Windows 10. Для работы с файлами json используется библиотека nlohmann. Для тестирования используется библиотека QTest. Для работы приложения в папке с исполняемым файлом должны находится файлы config.json, request.json и папка resource c файлами документов и запросов. 

- [Qt creator] - development environment!
- [json nlohman] - json free library
- [QTest] - tests integrated into the development environment

And of course SearchEngine itself is open source with a [public repository][dill]
 on GitHub.

## Installation

No installation required. 

## Plugins

The search engine is currently not extended with plugins.


#### Building for source

The project is built in the development environment and the executable file is launched.

## Docker

The search engine is not installed or deployed from a Docker container.

## License

MIT

**Free Software, Hell Yeah!**

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [dill]: <https://github.com/RomanLookin/SearchEngine>
