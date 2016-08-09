#include <catch.hpp>
#include <flow2d.hpp>
#include <cstdio>

USING_NS_FLOW2D;
USING_NS_FLOW2D_RESOURCE;
USING_NS_FLOW2D_FILESYSTEM;

TEST_CASE("PathConstruct")
{
    REQUIRE( (Path {"/"}) == "/" );
    REQUIRE( (Path {"///"}) == "/" );

    REQUIRE( (Path {"/resource"}) == "/resource" );
    REQUIRE( (Path {"/resource/img"}) == "/resource/img" );
    REQUIRE( (Path {"/resource/img/.."}) == "/resource" );
    REQUIRE( (Path {"/resource/img/../"}) == "/resource" );
    REQUIRE( (Path {"/resource/img/..///"}) == "/resource" );
    REQUIRE( (Path {"/../../resource/img/.."}) == "/resource" );
    REQUIRE( (Path {"/../../resource/img/..//"}) == "/resource" );
    REQUIRE( (Path {"/resource/img/../..///../.."}) == "/" );
    REQUIRE( (Path {"/../../resource/img/../..///../.."}) == "/" );
    REQUIRE( (Path {"/resource///"}) == "/resource" );
    REQUIRE( (Path {"/resource///img"}) == "/resource/img" );
    REQUIRE( (Path {"/resource///img////"}) == "/resource/img" );

    REQUIRE( (Path {"//resource"}) == "/resource" );
    REQUIRE( (Path {"//resource/img"}) == "/resource/img" );
    REQUIRE( (Path {"//resource/img/../"}) == "/resource" );
    REQUIRE( (Path {"//resource/img/../../"}) == "/" );
    REQUIRE( (Path {"//resource/img/../..///"}) == "/" );
    REQUIRE( (Path {"//../../resource/img/.."}) == "/resource" );
    REQUIRE( (Path {"//../../resource/img/..//"}) == "/resource" );
    REQUIRE( (Path {"//resource/img/../..///../.."}) == "/" );
    REQUIRE( (Path {"//../../resource/img/../..///../.."}) == "/" );
    REQUIRE( (Path {"//resource///"}) == "/resource" );

    REQUIRE( (Path {"resource"}) == "resource" );
    REQUIRE( (Path {"resource/img"}) == "resource/img" );
    REQUIRE( (Path {"resource/img/../"}) == "resource" );
    REQUIRE( (Path {"resource/img/../../"}) == "" );
    REQUIRE( (Path {"resource/img/../..///"}) == "" );
    REQUIRE( (Path {"../resource/img/.."}) == "../resource" );
    REQUIRE( (Path {"../../resource/img/.."}) == "../../resource" );
    REQUIRE( (Path {"../../resource/img/..//"}) == "../../resource" );
    REQUIRE( (Path {"resource/img/../..///.."}) == ".." );
    REQUIRE( (Path {"resource/img/../..///../.."}) == "../.." );
    REQUIRE( (Path {"../../resource/img/../..///../.."}) == "../../../.." );
    REQUIRE( (Path {"resource//img"}) == "resource/img" );
    REQUIRE( (Path {"resource//img////"}) == "resource/img" );
    REQUIRE( (Path {"resource//img////pic"}) == "resource/img/pic" );
}

TEST_CASE("PathConcatenationAndOthers")
{
    Path path("/");
    Path path2("resource");
    Path path3("nocompress/pic.png");

    REQUIRE( (path / path2) == "/resource" );
    REQUIRE( ((path / path2) / path3) == "/resource/nocompress/pic.png" );

    REQUIRE( (path  / Path("")) == "/" );
    REQUIRE( (path2 / Path("")) == "resource" );
    REQUIRE( (path3 / Path("")) == "nocompress/pic.png" );

    REQUIRE( (Path("") / path2) == "resource" );
    REQUIRE( (Path("") / path3) == "nocompress/pic.png" );

    REQUIRE( (Path("") / path2 / Path("")) == "resource" );
    REQUIRE( (Path("") / path3 / Path("")) == "nocompress/pic.png" );

    Path path4("/resource/img/hero_head.png");
    REQUIRE( path4.get_filename() == "hero_head.png" );
    REQUIRE( path4.get_basename() == "hero_head" );
    REQUIRE( path4.get_extension() == "png" );

    REQUIRE( path4.get_parent() == (Path {"/resource/img"}) );
    REQUIRE( path4.get_parent().get_parent() == (Path {"/resource"}) );
    REQUIRE( path4.get_root() == (Path {"/"}) );
    REQUIRE( path4.get_root().get_root() == (Path {"/"}) );
    REQUIRE( Path("resource").get_parent() == "" );
    REQUIRE( Path("").get_parent() == ".." );
    REQUIRE( Path("").get_parent().get_parent() == "../.." );
    REQUIRE( Path("").get_parent().get_parent().get_parent() == "../../.." );
    REQUIRE( Path("resource/img").get_root() == "resource" );
    REQUIRE( Path("resource").get_root() == "resource" );

    Path::iterator iterator = path4.begin();
    REQUIRE( *iterator++ == "resource" );
    REQUIRE( *iterator++ == "img" );
    REQUIRE( *iterator++ == "hero_head.png" );
    REQUIRE( iterator == path4.end() );
}

// TEST_CASE("Path")

// struct Text : public ResourceTrait<Text>
// {
//     bool load_from_file(DataStream::Ptr stream) override
//     {
//         char buffer[512];
//         stream->read(buffer, 512);
//         text = buffer;
//         return true;
//     };

//     size_t get_memory_usage() const override
//     {
//         return 1023;
//     }

//     std::string text;
// };

// struct ResourceCacheFixture
// {
//     ResourceCacheFixture() : cache(archives) {}
//     ArchiveManager          archives;
//     ResourceCacheManager    cache;
// };

TEST_CASE("TestFilesystem")
{
    REQUIRE( set_current_directory("../../test") );
    remove("tmp_dir");
    remove("resource", true);

    REQUIRE( create_directory("tmp_dir") );
    REQUIRE( is_directory("tmp_dir") );
    REQUIRE( !is_regular_file("tmp_dir") );

    REQUIRE( move("tmp_dir", "resource") );
    REQUIRE( !is_directory("tmp_dir") );
    REQUIRE( is_directory("resource") );

    auto file = open("resource/resource.txt", FileMode::APPEND);
    file.write("hahaha", 6);
    file.close();

    remove("tmp_dir");
    remove("resource", true);
    REQUIRE( !is_directory("tmp_dir") );
    REQUIRE( !is_directory("resource") );
}

// TEST_CASE_METHOD(ArchiveManager, "FilesystemArchive")
// {
//     auto s1 = open("resource.txt");
//     REQUIRE( !s1 );

//     add_archive<FilesystemArchive>();
//     auto s2 = open("resource.txt");
//     REQUIRE( s2 );
// }

// TEST_CASE_METHOD(ResourceCacheFixture, "")
// {
//     auto r = cache.get<Text>("./resource.txt");
//     REQUIRE( !r );

//     archives.add_archive<FilesystemArchive>();
//     r = cache.get<Text>("./resource.txt");
//     REQUIRE( r );

//     REQUIRE( r->text == "hahaha" );
//     REQUIRE( r.get_refcount() == 1 );

//     r.retain();
//     REQUIRE( r.get_refcount() == 2 );

//     auto r2 = cache.get<Text>("./resource.txt");
//     REQUIRE( r2 == r );
//     REQUIRE( r2.get_refcount() == 2 );

//     r2.retain();
//     REQUIRE( r2.get_refcount() == 3 );

//     auto r3 = cache.get<Text>("./resource2.txt");
//     REQUIRE( r3.get_refcount() == 1 );

//     r3.retain();

//     REQUIRE( cache.get_memory_usage() == 2046 );

//     r.release();
//     r2.release();

//     auto r4 = cache.get<Text>("./resource3.txt");
//     REQUIRE( cache.get_memory_usage() == 2046 );

//     r = cache.get<Text>("./resource.txt");
//     REQUIRE( cache.get_memory_usage() == 2046 );
//     r.retain();

//     r4 = cache.get<Text>("./resource3.txt");
//     REQUIRE( cache.get_memory_usage() == 3069 );
// };