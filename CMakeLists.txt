cmake_minimum_required(VERSION 3.10)

#プロジェクトと使用言語
project(Circles LANGUAGES C)

#オプション設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-Wall -Wextra")

#実行ファイル生成
add_executable(Circles index.c)

#ライブラリをリンク
find_package(raylib REQUIRED)  # CMakeでRaylibを探す
target_link_libraries(Circles PRIVATE raylib)  # リンクする

# macOSバンドルを有効にする
set_target_properties(Circles PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
)

# リソースファイルをバンドルに含める
set_source_files_properties(${CMAKE_SOURCE_DIR}/icon.icns PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
)