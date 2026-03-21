#pragma once

namespace codesh::external
{

class class_file_container_loader
{
protected:
    bool _is_loaded;
    class_file_container_loader();

public:
    virtual ~class_file_container_loader();

    /**
     * @return Whether the requested JAR is loaded fine
     */
    [[nodiscard]] bool is_loaded() const;
};

}
