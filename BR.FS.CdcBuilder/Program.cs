using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Configuration;


namespace BR.FS.CdcBuilder
{
    class Program
    {
        [DllImport("kernel32", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool SetDllDirectory(string lpPathName);

        private static void Main(string[] args)
        {
            bool setLibraryPathOk = false;
            bool setResourcePathOk = false;
            AppSettingsReader configReader = new AppSettingsReader();
            DirectoryInfo libraryPath = new DirectoryInfo((string)configReader.GetValue("library_path", typeof(string)));
            if (libraryPath.Exists)
            {
                setLibraryPathOk = SetDllDirectory(libraryPath.ToString());
            }
            else
            {
                Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now, "openCONFIGURATOR library path : " + libraryPath + " : does not exist"));
                return;
            }

            DirectoryInfo resourcePath = new DirectoryInfo((string)configReader.GetValue("resource_path", typeof(string)));
            if (resourcePath.Exists)
            {
                setResourcePathOk = openCONFIGURATORcsharpWRAPPER.SetLibraryResourcePath(resourcePath.ToString());
            }
            else
            {
                Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now, "openCONFIGURATOR resource path : " + resourcePath + " : does not exist"));
                return;
            }

            if (setResourcePathOk && setLibraryPathOk)
            {
                if (args.Length == 3)
                {
#if DEBUG
                    Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                        "Attach debugger and press any key"));
                    Console.ReadLine();
#endif
                    FileInfo projectFile = new FileInfo(args[0]);
                    String operatorOutput = args[1];
                    DirectoryInfo outputPath = new DirectoryInfo(args[2]);

                    if (projectFile.Exists == false)
                    {
                        Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                            "Error - Project file : " + projectFile + " does not exists"));
                    }

                    if (outputPath.Exists == false)
                    {
                        Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                            "Error - Output path : " + outputPath + " does not exists"));
                    }

                    if (String.CompareOrdinal(operatorOutput, "-o") != 0)
                    {
                        Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                            "Error - Wrong parameter : " + operatorOutput + " given"));
                    }

                    if (projectFile.Exists && String.CompareOrdinal(operatorOutput, "-o") == 0 && outputPath.Exists)
                    {
#if DEBUG
                        Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now, "Clean output folder"));
#endif
                        //Clean output path
                        foreach (FileInfo file in outputPath.GetFiles())
                        {
#if DEBUG
                            Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                "Delete file : " + file.DirectoryName.ToString() + Path.DirectorySeparatorChar +
                                file.ToString()));
#endif
                            if (String.CompareOrdinal(file.Extension, ".mak") != 0)
                            {
                                file.Delete();
                            }
                        }

                        ocfmRetCode retCode = openCONFIGURATORcsharpWRAPPER.OpenProject(projectFile.ToString());

#if DEBUG
                        Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                            "Open project : " + projectFile.ToString() + " : " + retCode.getErrorString() + " : " +
                            retCode.getErrorCode()));
#endif

                        if (retCode.getErrorCode() == ConfiguratorError.OCFM_ERR_SUCCESS)
                        {
                            retCode =
                                openCONFIGURATORcsharpWRAPPER.GenerateStackConfiguration(
                                    outputPath.ToString() + Path.DirectorySeparatorChar,
                                    "mnobd.cdc");
#if DEBUG
                            Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                "Generate stack configuration : " + outputPath.ToString() + Path.DirectorySeparatorChar +
                                "mnobd.cdc" + " : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
#endif
                            if (retCode.getErrorCode() != ConfiguratorError.OCFM_ERR_SUCCESS)
                            {
                                Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                    "Generate stack configuration failed : " + retCode.getErrorString()));
                            }
                            else
                            {
                                retCode =
                                    openCONFIGURATORcsharpWRAPPER.GenerateProcessImageDescription(OutputLanguage.CSHARP,
                                        outputPath.ToString() + Path.DirectorySeparatorChar, "PI.cs");
#if DEBUG
                                Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                    "GenerateProcessImageDescription for C# : " + outputPath.ToString() +
                                    Path.DirectorySeparatorChar +
                                    "PI.cs" + " : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
#endif
                                if (retCode.getErrorCode() != ConfiguratorError.OCFM_ERR_SUCCESS)
                                {
                                    Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                        "Generate C# process image failed: " + retCode.getErrorString()));
                                }
                                retCode =
                                    openCONFIGURATORcsharpWRAPPER.GenerateProcessImageDescription(OutputLanguage.XML,
                                        outputPath.ToString() + Path.DirectorySeparatorChar, "XAP.xml");
#if DEBUG
                                Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                    "GenerateProcessImageDescription for C : " + outputPath.ToString() +
                                    Path.DirectorySeparatorChar +
                                    "XAP.xml" + " : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
#endif
                                if (retCode.getErrorCode() != ConfiguratorError.OCFM_ERR_SUCCESS)
                                {
                                    Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                        "Generate C process image failed: " + retCode.getErrorString()));
                                }
                            }
                        }
                        else
                        {
                            Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                "Open Project failed: " + retCode.getErrorString()));
                        }

                        retCode = openCONFIGURATORcsharpWRAPPER.CloseProject();
#if DEBUG
                        Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                            "Close Project : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
#endif
                        if (retCode.getErrorCode() != ConfiguratorError.OCFM_ERR_SUCCESS)
                        {
                            Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                                "CloseProject : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
                        }
                    }
                }
                else
                {
                    Console.WriteLine("\nBernecker + Rainer " + Assembly.GetExecutingAssembly().GetName().Name + " " +
                                      Assembly.GetExecutingAssembly().GetName().Version);
                    Console.WriteLine("Usage: BR.FS.CdcBuilder.exe <ProjectFilePath> ... [options]");
                    Console.WriteLine("Options:");
                    Console.WriteLine(" -o <OutputPath>\tSet the output path for the generated files.\n");
                }
            }
            else
            {
                Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                    "Setting openCONFIGURATOR library path : " + libraryPath + " : succeeded : " + setLibraryPathOk));
                Console.WriteLine(System.String.Format("{0:G}: {1}.", System.DateTime.Now,
                    "Setting openCONFIGURATOR resource path : " + resourcePath + " : succeeded : " + setResourcePathOk));
            }
        }
    }
}
