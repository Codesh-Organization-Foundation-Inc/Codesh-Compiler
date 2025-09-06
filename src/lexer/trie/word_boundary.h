#pragma once

namespace codesh::lexer::trie
{

/**
 * Defines where letters *cannot* collide directly with a keyword.
 */
enum word_boundary
{
    BOTH, // Letters cannot collide anywhere near the keyword
    BEFORE, // Letters cannot collide before the keyword
    AFTER // Letters cannot collide after the keyword
};

}