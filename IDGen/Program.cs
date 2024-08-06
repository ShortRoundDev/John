using System.IO;
using System.Runtime.CompilerServices;

namespace IDGen
{
    internal static class Program
    {
        static Dictionary<int, string> entities = new Dictionary<int, string>();
        static Dictionary<int, string> tiles = new Dictionary<int, string>();

        static void Main(string[] args)
        {
            Console.WriteLine("Collecting entities");
            CollectIDs("Resources/entities.conf", "entities", entities);
            Console.WriteLine("Collecting tiles");
            CollectIDs("Resources/tiles.conf", "tiles", tiles);

            PrintDefs();
        }

        static void PrintDefHeaders(StreamWriter writer, Dictionary<int, string> def, string path)
        {
            writer.WriteLine("#pragma once\n");
            writer.WriteLine("#include \"JohnMapFile.h\"\n");
            writer.WriteLine("enum class " + path + " : uint16_t");
            writer.WriteLine("{");
            foreach (var id in def)
            {
                writer.WriteLine($"    {id.Value.FullID()} = {id.Key},");
            }
            writer.WriteLine("};\n");
        }
        static void PrintEntityDeclarations(StreamWriter writer)
        {
            foreach(var entity in entities)
            {
                writer.WriteLine($"IObject* Make{entity.Value.PascalCase()}(EntityToken const& token);");
            }
        }

        static void PrintDefs()
        {
            using (var writer = new StreamWriter($"Inc/EntDefs.h")) {
                // print working directory:
                Console.WriteLine("Writing ent defs to " + Path.GetFullPath("Inc/EntDefs.h"));
                PrintDefHeaders(writer, entities, "EntDefs");
                writer.WriteLine("class IObject;\n");
                writer.WriteLine();
                writer.WriteLine("IObject* CreateEntity(EntityToken const& token);");
                writer.Flush();
            }
            using (var writer = new StreamWriter($"./Inc/TileDefs.h"))
            {
                Console.WriteLine("Writing ent defs to " + Path.GetFullPath("Inc/TileDefs.h"));
                PrintDefHeaders(writer, tiles, "TileDefs");
                writer.WriteLine("class IBrush;");
                writer.WriteLine("BrushPtr CreateWall(WallToken const& token, int x, int y, Texture const* texture);");
                writer.WriteLine("IBrush* CreateFloor(WallToken const& token, int x, int y, Texture const* texture);");
                writer.WriteLine("IBrush* CreateCeiling(WallToken const& token, int x, int y, Texture const* texture);");
                writer.WriteLine();
                writer.Flush();
            }
            using (var writer = new StreamWriter($"Inc/EntDefs.cpp"))
            {
                Console.WriteLine("Writing ent defs to " + Path.GetFullPath("Inc/EntDefs.cpp"));
                writer.WriteLine("#include \"stdafx.h\"");
                writer.WriteLine("#include \"EntDefs.h\"");
                PrintEntityDeclarations(writer);
                PrintEntitySwitchCase(writer);
                writer.Flush();
            }
            using (var writer = new StreamWriter($"Inc/TileDefs.cpp"))
            {
                Console.WriteLine("Writing ent defs to " + Path.GetFullPath("Inc/TileDefs.cpp"));
                writer.WriteLine("#include \"stdafx.h\"");
                writer.WriteLine("#include \"TileDefs.h\"");
                writer.WriteLine("IBrush* MakeSimpleWall(WallToken const& token, int x, int y, Texture const* texture);");
                writer.WriteLine("IBrush* MakeSimpleFloor(WallToken const& token, int x, int y, Texture const* texture);");
                writer.WriteLine("IBrush* MakeSimpleCeiling(WallToken const& token, int x, int y, Texture const* texture);");
                PrintTileSwitchCase(writer, "Wall", "wallType");
                writer.WriteLine();
                PrintTileSwitchCase(writer, "Floor", "floor");
                writer.WriteLine();
                PrintTileSwitchCase(writer, "Ceiling", "ceiling");
                writer.Flush();
            }
        }

        static void PrintTileSwitchCase(StreamWriter writer, string name, string field)
        {
            writer.WriteLine($"IBrush* Create{name}(WallToken const& token, int x, int y, Texture const* texture)");
            writer.WriteLine("{");
            writer.WriteLine($"    switch (static_cast<TileDefs>(token.{field}))");
            writer.WriteLine("    {");
            foreach (var tile in tiles)
            {
                writer.WriteLine($"    case TileDefs::{tile.Value.FullID()}:");
            }
            writer.WriteLine("    default:");
            writer.WriteLine($"        return MakeSimple{name}(token, x, y, texture);");
            writer.WriteLine("    }");
            writer.WriteLine("    return nullptr;");
            writer.WriteLine("}");
        }

        static void PrintEntitySwitchCase(StreamWriter writer)
        {
            writer.WriteLine("IObject* CreateEntity(EntityToken const& token)");
            writer.WriteLine("{");
            writer.WriteLine("    switch (static_cast<EntDefs>(token.entityId))");
            writer.WriteLine("    {");
            foreach (var entity in entities)
            {
                writer.WriteLine($"    case EntDefs::{entity.Value.FullID()}:");
                writer.WriteLine($"        return Make{entity.Value.PascalCase()}(token);");
            }
            writer.WriteLine("    }");
            writer.WriteLine("    return nullptr;");
            writer.WriteLine("}");
        }

        static void CollectIDs(string fileName, string path, Dictionary<int, string> def)
        {
            var entities = File.ReadAllLines(fileName);
            foreach (var entity in entities)
            {
                var config = File.ReadAllLines($"Resources/{path}/{entity}.conf");
                var idStr = config.Where(config => config.StartsWith("#id=")).FirstOrDefault()?.Split("=");
                if (idStr == null || idStr.Length < 2)
                {
                    continue;
                }
                var id = UInt16.Parse(idStr[1]);
                def.Add(id, entity.PascalCase());
            }
        }

        static string FullID(this string entity)
        {
            return $"ID_{entity.ToUpper()}";
        }

        static string PascalCase(this string str)
        {
            return str[0].ToString().ToUpper() + str.Substring(1);
        }
    }
}
