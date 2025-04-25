add_executable(log log.cc)
target_link_libraries(log tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(log PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# 在所有属性设置完成后查询
if(TARGET tool)
    get_target_property(tool_libs tool LINK_LIBRARIES)
    get_target_property(tool_libs_interface tool INTERFACE_LINK_LIBRARIES)
    get_target_property(tool_includes tool INTERFACE_INCLUDE_DIRECTORIES)

    # tool目标直接依赖spdlog::spdlog库
    # 来源：target_link_libraries(tool PRIVATE spdlog::spdlog)
    # ​影响范围：仅影响tool自身的编译和链接
    if(tool_libs)
        message(STATUS "Tool links with: ${tool_libs}")
    else()
        message(WARNING "Tool link libraries not found")
    endif()

    # 编译阶段：不传播给依赖者（如log目标）
    # ​链接阶段：仍会参与最终链接
    if(tool_libs_interface)
        message(STATUS "Tool interface links with: ${tool_libs_interface}")
    else()
        message(WARNING "Tool interface link libraries not found")
    endif()

    # 这些路径会自动传播给所有链接tool的目标（如log）
    # 在log目标的编译命令中会自动添加-I参数包含这些路径
    # 来源：target_include_directories(tool PUBLIC ${MODULES_SOURCE_DIR})
    if(tool_includes)
        message(STATUS "Tool includes: ${tool_includes}")
    else()
        message(WARNING "Tool include directories not found")
    endif()
endif()

# 对log目标的
# ​(1) 编译阶段**
# ​获得：tool的INTERFACE_INCLUDE_DIRECTORIES（即所有PUBLIC包含路径）
# ​不获得：spdlog的头文件路径（因为spdlog是PRIVATE依赖）
# ​(2) 链接阶段
# ​显式链接：tool库
# ​隐式链接：通过tool传递的$<LINK_ONLY:spdlog::spdlog>，最终会链接spdlog
