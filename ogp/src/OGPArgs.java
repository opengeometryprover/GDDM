package ogp;

import java.io.File;

public class OGPArgs {

    private String conjectureId;
    private String conjectureFormat;
    private String proverId;
    private String proverArgs;
    private boolean tgtp = false;
    private boolean help = false;
    private boolean provers = false;
    private boolean version = false;

    public OGPArgs(String[] args) {
	switch (args.length) {
	case 0:
	    errorMsg(1, "");
	    break;
	case 1:
	    help = args[0].equals("-h") || args[0].equals("--help");
	    provers = args[0].equals("-p") || args[0].equals("--provers");
	    version = args[0].equals("-V") || args[0].equals("--version");
	    if (!help && !provers && !version) {
		conjectureTest(args[0]);
	    }
	    break;
	}
    }

    public String getConjectureId() {
	return this.conjectureId;
    }

    public String getConjectureFormat() {
	return this.conjectureFormat;
    }

    public String getProverId() {
	returb this.proverId;
    }

    public String getProverArgs() {
	return this.proverArgs;
    }

    public boolean getTGTP() {
	return this.tgtp;
    }

    public boolean getHelp() {
	return this.help;
    }

    public boolean getProvers() {
	return this.provers;
    }

    public boolean getVersion() {
	return this.version;
    }

    private void conjectureTest(String conjecture) {
	tgtp = conjecture.startsWith("--tgtp=");
	if (tgtp) {
	    conjectureId = conjecture.substring(7);
	    conjectureFormat = "";
	    if (conjectureId.isEmpty()) {
		errorMsg(2, "");
	    }
	    // TODO: Must check if conjecture exists in TGTP
	} else {
	    File theConjecture = new File(conjecture);
	    if (!theConjecture.exists()) {
		errorMsg(3, conjecture);
	    } else if (!theConjecture.isFile()) {
		errorMsg(4, conjecture);
	    } else if (!theConjecture.canRead()) {
		errorMsg(5, conjecture);
	    } else {
		conjectureId = conjecture;
	    }
	}
    }

    private static void errorMsg(int error, String str) {
	System.err.print("[ERROR " + error + "] ");
	switch (error) {
	case 1:
	    System.err.print("Incorrect number of arguments.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	case 2:
	    System.err.println("Empty TGTP conjecture.");
	    break;
	case 3:
	    System.err.println("File '" + str + "' does not exist.");
	    break;
	case 4:
	    System.err.println("'" + str + "' is not a file.");
	    break;
	case 5:
	    System.err.println("Cannot read file '" + str + "'.");
	    break;
	}
	System.exit(error);
    }

}
