#pragma once

namespace codesh::definition
{
class fully_qualified_name;
}
namespace codesh::semantic_analyzer
{
class symbol_table;
}


namespace codesh::external
{

class class_loader
{
protected:
    bool _is_loaded;
    class_loader();

public:
    virtual ~class_loader();

    /**
     * @returns Whether the requested JAR is loaded fine
     */
    [[nodiscard]] bool is_loaded() const;

    /**
     * Loads all symbols of an external class file to the provided @p table
     *
     * @returns Whether a class was successfully loaded
     */
    virtual bool load(const semantic_analyzer::symbol_table &symbol_table,
                      const definition::fully_qualified_name &class_name) = 0;
};

}
