add_rules("mode.release", "mode.debug")

target("GetColor")
    add_rules("qt.widgetapp")
    add_cxflags(
        "/utf-8"
    )
    add_defines(
        "NOMINMAX",
        "UNICODE"
    )
    add_files(
        "**.cpp",
        "**.h",
        "**.ui",
        "**.qrc",
        "**.rc"
    )
    add_includedirs(".")
    add_headerfiles("*.h")

    if is_mode("debug") then
        set_symbols("debug")
    end

    after_build(function (target) 
        -- copy cursor.png to build target directory
        os.cp("cursor.png", path.join(target:targetdir(), "cursor.png"))
    end)
