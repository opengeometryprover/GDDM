import java.io.File;

public class OGPTestArgs {

    private String file;
    private boolean help = false;
    private int timeout = 5;
    private boolean verbose = false;
    private boolean version = false;

    public OGPTestArgs(String[] args) {
	int i;

	switch (args.length) {
	case 0:
	    errorMsg(1, "");
	    break;
	case 1:
	    help = args[0].equals("-h") || args[0].equals("--help");
	    version = args[0].equals("-V") || args[0].equals("--version");
	    if (!help && !version) {
		testFile(args[0]);
	    }
	    break;
	default:
	    testFile(args[0]);
	    i = 1;
	    while (i <= args.length - 1) {
		if (args[i].equals("-t")
		    || args[i].startsWith("--timeout")) {
		    if (args[i].equals("-t")) {
			if (i == args.length - 1) {
			    errorMsg(5, "");
			} else {
			    try {
				timeout = Integer.parseUnsignedInt(args[++i]);
			    } catch(NumberFormatException e) {
				errorMsg(6, "");
			    }
			}
		    }
		} else if (args[i].equals("-v")
			   || args[i].startsWith("--verbose")) {
		    verbose = true;
		} else {
		    errorMsg(7, args[i]);
		}
		i++;
	    }
	}
    }

    public String getFile() {
	return this.file;
    }

    public boolean getHelp() {
	return this.help;
    }

    public int getTimeout() {
	return this.timeout;
    }

    public boolean getVerbose() {
	return this.verbose;
    }

    public boolean getVersion() {
	return this.version;
    }

    private void testFile(String fileStr) {
	File theFile = new File(fileStr);
	if (!theFile.exists()) {
	    errorMsg(2, fileStr);
	} else if (!theFile.isFile()) {
	    errorMsg(3, fileStr);
	} else if (!theFile.canRead()) {
	    errorMsg(4, fileStr);
	} else {
	    file = fileStr;
	}
    }

    private static void errorMsg(int error, String str) {
	System.err.print("[ERROR] ");
	switch (error) {
	case 1:
	    System.err.print("Incorrect number of arguments.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	case 2:
	    System.err.println("File '" + str + "' does not exist.");
	    break;
	case 3:
	    System.err.println("'" + str + "' is not a file.");
	    break;
	case 4:
	    System.err.println("Cannot read file '" + str + "'.");
	    break;
	case 5:
	    System.err.println("Timeout option is missing its integer value");
	    break;
	case 6:
	    System.err.println("Timeout must be an integer.");
	    break;
	case 7:
	    System.err.print("Unrecognized option '" + str + "'.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	}
	System.exit(error);
    }

}
