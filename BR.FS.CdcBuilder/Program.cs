using System;
using System.ComponentModel;
using System.Configuration;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using ExtensionMethods;

namespace ExtensionMethods
{

    public static class Extensions
    {

        /// <summary>
        ///  Retrieve the value of the Description-Attribute assigned to an enum constant and format
        ///  it using the specified arguments.
        /// </summary>
        /// <param name="value">enum constant to get description for.</param> 
        /// <param name="args">Formatting objects to format description string.</param>
        /// <returns>A formatted description string or <code>null</code> if it does not have an assigned Description-Attribute.</returns>
        public static string GetDescription(this Enum value, params object[] args)
        {
            Type type = value.GetType();
            string name = Enum.GetName(type, value);
            if (name != null)
            {
                FieldInfo field = type.GetField(name);
                if (field != null)
                {
                    DescriptionAttribute attr =
                           Attribute.GetCustomAttribute(field, typeof(DescriptionAttribute)) as DescriptionAttribute;
                    if (attr != null)
                        return String.Format(attr.Description, args);
                }
            }
            return null;
        }

    }// Extensions

}// ExtensionMethods

namespace BR.FS.CdcBuilder
{
    class Program
    {
        private const string CDC_FILENAME = "mnobd.cdc";
        private const string PI_CSHARP_FILENAME = "ProcessImage.cs";
        private const string PI_XML_FILENAME = "XAP.xml";

        [DllImport("kernel32", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool SetDllDirectory(string lpPathName);

        private enum ASErrorSeverity { ERROR, WARNING };

        private enum BuilderError
        {
            [Description("Invalid number of arguments. Expected: {0:d}, actual: {1:d}.")]
            USAGE_INVALID_NO_OF_ARGUMENTS,
            [Description("Invalid option '{0}'.")]
            USAGE_INVALID_OPTION,
            [Description("Project file '{0}' does not exist.")]
            PROJECTFILE_NOT_EXISTING,
            [Description("Output path '{0}' does not exist.")]
            OUTPUTPATH_NOT_EXISTING
        };

        private enum ReturnCode { SUCCESS = 0, WARNING = 1, ERROR = 2, FATAL_ERROR = 3 };

        private static string GetFormattedErrorMsg(ASErrorSeverity severity, BuilderError errorNo, string msg)
        {
            return String.Format("{0} {1:d}: {2}", severity.ToString("G").ToLower(), errorNo, msg);
        }

        private static void PrintUsageMessage()
        {
            Console.WriteLine("\nBernecker + Rainer "
                    + Assembly.GetExecutingAssembly().GetName().Name + " "
                    + Assembly.GetExecutingAssembly().GetName().Version);
            Console.WriteLine("Usage: BR.FS.CdcBuilder.exe <ProjectFilePath> [options]");
            Console.WriteLine("Options:");
            Console.WriteLine(" -o <OutputPath>\tOutput path for generated files.\n");
        }

        private static int Main(string[] args)
        {
            AppSettingsReader configReader = new AppSettingsReader();
            DirectoryInfo libraryPath = new DirectoryInfo((string)configReader.GetValue("library_path", typeof(string)));
            if (libraryPath.Exists)
            {
                if (!SetDllDirectory(libraryPath.ToString()))
                    throw new ConfigurationErrorsException("Configured openCONFIGURATOR library path '"
                        + libraryPath.FullName
                        + "' could not be set as DLL search path.\n"
                        + "SetDllDirectory(path) failed.");
            }
            else
                throw new ConfigurationErrorsException("Configured openCONFIGURATOR library path '" + libraryPath.FullName + "' does not exist.");

            DirectoryInfo resourcePath = new DirectoryInfo((string)configReader.GetValue("resource_path", typeof(string)));
            if (resourcePath.Exists)
                Environment.SetEnvironmentVariable("OPENCONFIGURATOR_LIBRARY_RESOURCEPATH", resourcePath.ToString());
            else
                throw new ConfigurationErrorsException("Configured openCONFIGURATOR resource path '" + resourcePath.FullName + "' does not exist.");

            if (args.Length == 3)
            {
#if DEBUG
                Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                    "Attach debugger and press any key"));
                Console.ReadLine();
#endif
                FileInfo projectFile = new FileInfo(args[0]);
                String outputOption = args[1];
                DirectoryInfo outputPath = new DirectoryInfo(args[2]);

                if (!projectFile.Exists)
                {
                    Console.WriteLine(GetFormattedErrorMsg(
                        ASErrorSeverity.ERROR,
                        BuilderError.PROJECTFILE_NOT_EXISTING,
                        BuilderError.PROJECTFILE_NOT_EXISTING.GetDescription(projectFile.FullName)));
                    return (int)ReturnCode.FATAL_ERROR;
                }

                if (!outputPath.Exists)
                {
                    Console.WriteLine(GetFormattedErrorMsg(
                        ASErrorSeverity.ERROR,
                        BuilderError.OUTPUTPATH_NOT_EXISTING,
                        BuilderError.OUTPUTPATH_NOT_EXISTING.GetDescription(outputPath.FullName)));
                    return (int)ReturnCode.FATAL_ERROR;
                }

                if (outputOption != "-o")
                {
                    Console.WriteLine(GetFormattedErrorMsg(
                        ASErrorSeverity.ERROR,
                        BuilderError.USAGE_INVALID_OPTION,
                        BuilderError.USAGE_INVALID_OPTION.GetDescription(outputOption)));
                    return (int)ReturnCode.FATAL_ERROR;
                }

#if DEBUG
                Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now, "Cleaning output folder " + outputPath.FullName));
#endif
                //Clean output path
                foreach (FileInfo file in outputPath.GetFiles())
                {
                    if (file.Extension != ".mak")
                    {
#if DEBUG
                        Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now, "Deleting file : " + file.FullName));
#endif
                        file.Delete();
                    }
                }

                ocfmRetCode retCode = openCONFIGURATORcsharpWRAPPER.OpenProject(projectFile.ToString());

#if DEBUG
                Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                    "Open project : " + projectFile.ToString() + " : " + retCode.getErrorString() + " : " +
                    retCode.getErrorCode()));
#endif

                if (retCode.getErrorCode() == ConfiguratorError.OCFM_ERR_SUCCESS)
                {
                    retCode =
                        openCONFIGURATORcsharpWRAPPER.GenerateStackConfiguration(
                            outputPath.ToString() + Path.DirectorySeparatorChar,
                            Program.CDC_FILENAME);
#if DEBUG
                    Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                        "Generate stack configuration : " + outputPath.ToString() + Path.DirectorySeparatorChar +
                        Program.CDC_FILENAME + " : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
#endif
                    if (retCode.getErrorCode() != ConfiguratorError.OCFM_ERR_SUCCESS)
                    {
                        Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                            "Generate stack configuration failed : " + retCode.getErrorString()));
                        return (int)ReturnCode.FATAL_ERROR;
                    }
                    else
                    {
                        retCode =
                            openCONFIGURATORcsharpWRAPPER.GenerateProcessImageDescription(OutputLanguage.CSHARP,
                                outputPath.ToString() + Path.DirectorySeparatorChar, Program.PI_CSHARP_FILENAME);
#if DEBUG
                        Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                            "GenerateProcessImageDescription for C# : " + outputPath.ToString() +
                            Path.DirectorySeparatorChar +
                            Program.PI_CSHARP_FILENAME + " : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
#endif
                        if (retCode.getErrorCode() != ConfiguratorError.OCFM_ERR_SUCCESS)
                        {
                            Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                                "Generate C# process image failed: " + retCode.getErrorString()));
                            return (int)ReturnCode.FATAL_ERROR;
                        }
                        retCode =
                            openCONFIGURATORcsharpWRAPPER.GenerateProcessImageDescription(OutputLanguage.XML,
                                outputPath.ToString() + Path.DirectorySeparatorChar, PI_XML_FILENAME);
#if DEBUG
                        Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                            "GenerateProcessImageDescription for C : " + outputPath.ToString() +
                            Path.DirectorySeparatorChar +
                            PI_XML_FILENAME + " : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
#endif
                        if (retCode.getErrorCode() != ConfiguratorError.OCFM_ERR_SUCCESS)
                        {
                            Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                                "Generate C process image failed: " + retCode.getErrorString()));
                            return (int)ReturnCode.FATAL_ERROR;
                        }
                    }
                }
                else
                {
                    Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                        "Open Project failed: " + retCode.getErrorString()));
                    return (int)ReturnCode.FATAL_ERROR;
                }

                retCode = openCONFIGURATORcsharpWRAPPER.CloseProject();
#if DEBUG
                Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                    "Close Project : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
#endif
                if (retCode.getErrorCode() != ConfiguratorError.OCFM_ERR_SUCCESS)
                {
                    Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                        "CloseProject : " + retCode.getErrorString() + " : " + retCode.getErrorCode()));
                    return (int)ReturnCode.FATAL_ERROR;
                }
                return (int)ReturnCode.SUCCESS;
            }
            else
            {
                Console.WriteLine(GetFormattedErrorMsg(
                    ASErrorSeverity.ERROR,
                    BuilderError.USAGE_INVALID_NO_OF_ARGUMENTS,
                    BuilderError.USAGE_INVALID_NO_OF_ARGUMENTS.GetDescription(3, args.Length)));
                PrintUsageMessage();
                return (int)ReturnCode.FATAL_ERROR;
            }
        }
    }
}
