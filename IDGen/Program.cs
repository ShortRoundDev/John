using System.IO;
using System.Runtime.CompilerServices;

namespace IDGen
{
    internal static class Program
    {
        static Dictionary<int, string> entities = new Dictionary<int, string>();
        static Dictionary<int, (string, string?)> tiles = new Dictionary<int, (string, string?)>();

        static void Main(string[] args)
        {
            Console.WriteLine("Collecting entities");
            CollectIDs("Resources/entities.conf", "entities", entities);
            Console.WriteLine("Collecting tiles");
            CollectTiles("Resources/tiles.conf", "tiles", tiles);

            PrintDefs();
        }

        static void PrintDefHeaders(StreamWriter writer, IEnumerable<(int, string)> def, string path)
        {
            writer.WriteLine("#pragma once\n");
            writer.WriteLine("#include \"JohnMapFile.h\"\n");
            writer.WriteLine("enum class " + path + " : uint16_t");
            writer.WriteLine("{");
            foreach (var id in def)
            {
                writer.WriteLine($"    {id.Item2.FullID()} = {id.Item1},");
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
                PrintDefHeaders(writer, entities.Select(kvp => (kvp.Key, kvp.Value)), "EntDefs");
                writer.WriteLine("class IObject;\n");
                writer.WriteLine();
                writer.WriteLine("IObject* CreateEntity(EntityToken const& token);");
                writer.Flush();
            }
            using (var writer = new StreamWriter($"./Inc/TileDefs.h"))
            {
                Console.WriteLine("Writing ent defs to " + Path.GetFullPath("Inc/TileDefs.h"));
                PrintDefHeaders(writer, tiles.Select(kvp => (kvp.Key, kvp.Value.Item2 ?? kvp.Value.Item1)), "TileDefs");
                writer.WriteLine("class IBrush;");
                writer.WriteLine("IBrush* CreateWall(WallToken const& token, int x, int y, TextureCPtr texture);");
                writer.WriteLine("IBrush* CreateFloor(WallToken const& token, int x, int y, TextureCPtr texture);");
                writer.WriteLine("IBrush* CreateCeiling(WallToken const& token, int x, int y, TextureCPtr texture);");
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
                writer.WriteLine("IBrush* MakeSimpleWall(WallToken const& token, int x, int y, TextureCPtr texture);");
                writer.WriteLine("IBrush* MakeSimpleFloor(WallToken const& token, int x, int y, TextureCPtr texture);");
                writer.WriteLine("IBrush* MakeSimpleCeiling(WallToken const& token, int x, int y, TextureCPtr texture);");
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
            var custom = tiles.Where(tile => tile.Value.Item2 != null);
            foreach (var tile in custom)
            {
                writer.WriteLine("IBrush* Make" + tile.Value.Item2!.PascalCase() + "(WallToken const& token, int x, int y);");
            }

            writer.WriteLine($"IBrush* Create{name}(WallToken const& token, int x, int y, TextureCPtr texture)");
            writer.WriteLine("{");
            writer.WriteLine($"    switch (static_cast<TileDefs>(token.{field}))");
            writer.WriteLine("    {");

            foreach(var tile in custom)
            {
                writer.WriteLine($"    case TileDefs::{tile.Value.Item2!.FullID()}:");
                    writer.WriteLine($"        return Make{tile.Value.Item2!.PascalCase()}(token, x, y);");
            }

            var nonCustom = tiles.Where(tile => tile.Value.Item2 == null);
            foreach (var tile in nonCustom)
            {
                writer.WriteLine($"    case TileDefs::{tile.Value.Item1.FullID()}:");
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

        static void CollectTiles(string fileName, string path, Dictionary<int, (string, string?)> def)
        {
            var entities = File.ReadAllLines(fileName);
            foreach (var entity in entities)
            {
                var config = File.ReadAllLines($"Resources/{path}/{entity}.conf");
                var idStr = config.Where(config => config.StartsWith("#id=")).FirstOrDefault()?.Split("=");
                var custom = config.Where(config => config.StartsWith("#custom=")).FirstOrDefault()?.Split("=");
                if (idStr == null || idStr.Length < 2)
                {
                    continue;
                }
                var id = UInt16.Parse(idStr[1]);
                var customValue = (custom?.Length ?? 0) > 1 ? custom![1] : null;
                def.Add(id, (entity.PascalCase(), customValue));
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
