using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lr1
{
    class Program
    {
        public const string SEARCHPATH = @"..\..\..\";
        public const string WRITEPATH = @"..\..\result.txt";

        static void Main(string[] args)
        {
            Console.WriteLine($"Searching files in path: {SEARCHPATH}");
            string[] result = Directory.GetDirectories(SEARCHPATH);
            string printRes = "\nFollowing files were found:\n";
            List<string> allFiles = new List<string>();
            List<string> resultText = new List<string>();

            foreach (string folder in result)
            {
                try
                {
                    Console.WriteLine($"Searching in {folder} ...");
                    string[] files = Directory.GetFiles(folder, "*.txt", SearchOption.AllDirectories);
                    foreach (string file in files)
                    {
                        printRes += file + "\n";
                        allFiles.Add(file);
                    }
                    Console.WriteLine("Done");
                }
                catch
                {
                    Console.WriteLine("Access denied: " + folder);
                }
            }

            if (allFiles.Count != 0)
                Console.WriteLine(printRes);
            else Console.WriteLine("\nNo content found");

            foreach (string file in allFiles)
            {
                resultText.AddRange(File.ReadAllLines(file));
            }
            File.AppendAllLines(WRITEPATH, resultText);

            Console.ReadKey();
        }
    }
}
