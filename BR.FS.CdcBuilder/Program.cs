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
            USAGE_INVALID_NO_OF_ARGUMENTS = 5400,
            [Description("Invalid option '{0}'.")]
            USAGE_INVALID_OPTION = 5401,
            [Description("Project file '{0}' does not exist.")]
            PROJECTFILE_NOT_EXISTING = 5402,
            [Description("Output path '{0}' does not exist.")]
            OUTPUTPATH_NOT_EXISTING = 5403
        };

        private enum ReturnCode { SUCCESS = 0, WARNING = 1, ERROR = 2, FATAL_ERROR = 3 };

        private static string GetFormattedErrorMsg(ASErrorSeverity severity, BuilderError errorNo, string msg)
        {
            return String.Format("{0} {1:d}: {2}", severity.ToString("G").ToLower(), errorNo, msg);
        }

        private static string GetFormattedErrorMsg(ASErrorSeverity severity, Result res)
        {
            return String.Format("{0} {1:d}: {2}", severity.ToString("G").ToLower(), GetASErrorNr(res.GetErrorCode()), res.GetErrorString());
        }

        private static int GetASErrorNr(ErrorCode code)
        {
            switch (code)
            {
                case ErrorCode.FILE_WRITE_FAILED:
                    return 5410;
                case ErrorCode.FILE_READ_FAILED:
                    return 5411;
                case ErrorCode.LEGACY_ERROR:
                    return 5412;
                case ErrorCode.SUBINDEX_INVALID:
                    return 5413;
                case ErrorCode.INDEX_INVALID:
                    return 5414;
                case ErrorCode.NODEID_INVALID:
                    return 5415;
                case ErrorCode.ATTRIBUTEVALUE_INVALID:
                    return 5416;
                case ErrorCode.ATTRIBUTEVALUE_NOT_IN_RANGE:
                    return 5417;
                case ErrorCode.UNSUPPORTED_ATTRIBUTETYPE:
                    return 5418;
                case ErrorCode.NO_NODES_CONFIGURED:
                    return 5419;
                case ErrorCode.NO_CONTROLLED_NODES_CONFIGURED:
                    return 5420;
                case ErrorCode.NO_MANAGING_NODE_CONFIGURED:
                    return 5421;
                case ErrorCode.OD_EMPTY:
                    return 5422;
                case ErrorCode.INDEX_CONTAINS_NO_SUBINDICES:
                    return 5423;
                case ErrorCode.NODE_DOES_NOT_EXIST:
                    return 5424;
                case ErrorCode.INDEX_DOES_NOT_EXIST:
                    return 5425;
                case ErrorCode.SUBINDEX_DOES_NOT_EXIST:
                    return 5426;
                case ErrorCode.NODE_EXISTS:
                    return 5427;
                case ErrorCode.INDEX_EXISTS:
                    return 5428;
                case ErrorCode.SUBINDEX_EXISTS:
                    return 5429;
                case ErrorCode.EXTERNAL_SYSTEM_CALL_FAILED:
                    return 5430;
                case ErrorCode.TPDO_CHANNEL_COUNT_EXCEEDED:
                    return 5431;
                case ErrorCode.NODE_CONFIGURATION_ERROR:
                    return 5432;
                case ErrorCode.MAPPED_INDEX_DOES_NOT_EXIST:
                    return 5433;
                case ErrorCode.MAPPED_SUBINDEX_DOES_NOT_EXIST:
                    return 5434;
                case ErrorCode.INSUFFICIENT_MAPPING_OBJECTS:
                    return 5435;
                case ErrorCode.PARAMETER_NOT_FOUND:
                    return 5436;
                case ErrorCode.STRUCT_DATATYPE_NOT_FOUND:
                    return 5437;
                case ErrorCode.SIMPLE_DATATYPE_NOT_FOUND:
                    return 5438;
                case ErrorCode.VALUE_NOT_WITHIN_RANGE:
                    return 5439;
                case ErrorCode.CHANNEL_PAYLOAD_LIMIT_EXCEEDED:
                    return 5440;
                case ErrorCode.CHANNEL_OBJECT_LIMIT_EXCEEDED:
                    return 5441;
                case ErrorCode.PDO_DATATYPE_INVALID:
                    return 5442;
                case ErrorCode.UNSUPPORTED_PI_GEN_LANGUAGE:
                    return 5443;
                case ErrorCode.MAX_PI_SIZE_EXCEEDED:
                    return 5444;
                case ErrorCode.MULTIPLEXING_NOT_SUPPORTED:
                    return 5445;
                case ErrorCode.MULTIPLEX_CYCLE_ASSIGN_INVALID:
                    return 5446;
                case ErrorCode.OBJECT_LIMITS_INVALID:
                    return 5447;
                case ErrorCode.LOW_CN_PRES_TIMEOUT:
                    return 5448;
                case ErrorCode.CROSS_TRAFFIC_STATION_LIMIT_EXCEEDED:
                    return 5449;
                case ErrorCode.ARGUMENT_INVALID_NULL:
                    return 5450;
                case ErrorCode.UNHANDLED_EXCEPTION:
                    return 5451;
                case ErrorCode.NO_DEFAULT_OR_ACTUAL_VALUE:
                    return 5452;
                case ErrorCode.FEATURE_VALUE_NOT_FOUND:
                    return 5453;
                case ErrorCode.PLKDATATYPE_SIZE_UNDEFINED:
                    return 5454;
                case ErrorCode.OBJECT_SIZE_MAPPED_INVALID:
                    return 5455;
                case ErrorCode.ARGUMENT_INVALID_EMPTY:
                    return 5456;
                case ErrorCode.MAPPING_TYPE_FOR_PDO_INVALID:
                    return 5457;
                case ErrorCode.ACCESS_TYPE_FOR_PARAMETER_INVALID:
                    return 5458;
                case ErrorCode.ACCESS_TYPE_FOR_PDO_INVALID:
                    return 5459;
                case ErrorCode.PDO_OFFSET_INVALID:
                    return 5460;
                case ErrorCode.XML_INVALID:
                    return 5461;
                case ErrorCode.SCHEMA_NOT_FOUND_OR_WELL_FORMED:
                    return 5462;
                case ErrorCode.SCHEMA_INVALID:
                    return 5463;
                case ErrorCode.SCHEMA_PARSER_CONTEXT_ERROR:
                    return 5464;
                case ErrorCode.SCHEMA_VALIDATION_CONTEXT_ERROR:
                    return 5465;
                case ErrorCode.NO_PROJECT_LOADED:
                    return 5466;
                case ErrorCode.MAPPING_INVALID:
                    return 5467;
                case ErrorCode.PARAMETER_VALUE_NOT_SET:
                    return 5468;
                case ErrorCode.PARAMETER_VALUE_INVALID:
                    return 5469;
                default:
                    return 0;
            }
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
                //Console.ReadLine();
#endif
                FileInfo projectFile = new FileInfo(args[0].Replace("\\ ", " "));
                String outputOption = args[1];
                DirectoryInfo outputPath = new DirectoryInfo(args[2].Replace("\\ ", " "));
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

                Result retCode = openCONFIGURATORcsharpWRAPPER.OpenProject(projectFile.ToString());

#if DEBUG
                Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                    "Open project : " + projectFile.ToString() + " : " + retCode.GetErrorString() + " : " +
                    retCode.GetErrorCode()));
#endif

                if (retCode.IsSuccessful())
                {
                    retCode =
                        openCONFIGURATORcsharpWRAPPER.GenerateStackConfiguration(
                            outputPath.ToString() + Path.DirectorySeparatorChar,
                            Program.CDC_FILENAME);
#if DEBUG
                    Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                       "Generate stack configuration : " + outputPath.ToString() + Path.DirectorySeparatorChar +
                       Program.CDC_FILENAME + " : " + retCode.GetErrorString() + " : " + retCode.GetErrorCode()));
#endif
                    if (!retCode.IsSuccessful())
                    {
                        Console.WriteLine(GetFormattedErrorMsg(ASErrorSeverity.ERROR, retCode));
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
                            Program.PI_CSHARP_FILENAME + " : " + retCode.GetErrorString() + " : " + retCode.GetErrorCode()));
#endif
                        if (!retCode.IsSuccessful())
                        {
                            Console.WriteLine(GetFormattedErrorMsg(ASErrorSeverity.ERROR, retCode));
                            return (int)ReturnCode.FATAL_ERROR;
                        }
                        retCode =
                            openCONFIGURATORcsharpWRAPPER.GenerateProcessImageDescription(OutputLanguage.XML,
                                outputPath.ToString() + Path.DirectorySeparatorChar, PI_XML_FILENAME);
#if DEBUG
                        Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                            "GenerateProcessImageDescription for C : " + outputPath.ToString() +
                            Path.DirectorySeparatorChar +
                            PI_XML_FILENAME + " : " + retCode.GetErrorString() + " : " + retCode.GetErrorCode()));
#endif
                        if (!retCode.IsSuccessful())
                        {
                            Console.WriteLine(GetFormattedErrorMsg(ASErrorSeverity.ERROR, retCode));
                            return (int)ReturnCode.FATAL_ERROR;
                        }
                    }
                }
                else
                {
                    Console.WriteLine(GetFormattedErrorMsg(ASErrorSeverity.ERROR, retCode));
                    return (int)ReturnCode.FATAL_ERROR;
                }

                retCode = openCONFIGURATORcsharpWRAPPER.CloseProject();
#if DEBUG
                Console.WriteLine(String.Format("{0:G}: {1}.", DateTime.Now,
                    "Close Project : " + retCode.GetErrorString() + " : " + retCode.GetErrorCode()));
#endif
                if (!retCode.IsSuccessful())
                {
                    Console.WriteLine(GetFormattedErrorMsg(ASErrorSeverity.ERROR, retCode));
                    return (int)ReturnCode.FATAL_ERROR;
                }
                //AS output when build finished successful
                Console.WriteLine("Generating POWERLINK stack-configuration to: " + outputPath.FullName + Path.DirectorySeparatorChar + " ...");
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
