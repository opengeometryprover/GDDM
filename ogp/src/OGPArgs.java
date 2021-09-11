package ogp;

import java.io.File;

public class OGPArgs {

    private int timeout = 60;
    private String conjName;
    private String conjExt;
    private String proverId;
    private String proverArgs;
    private String toFOFCmd;
    private boolean tgtp = false;
    private boolean help = false;
    private boolean provers = false;
    private boolean version = false;

    public OGPArgs(String[] args, OGPConf configuration) {
	switch (args.length) {
	case 0:
	    // No arguments
	    errorMsg(201, "");
	    break;
	case 1:
	    // ogp -h | --help
	    help = args[0].equals("-h") || args[0].equals("--help");
	    // ogp -p | --provers
	    provers = args[0].equals("-p") || args[0].equals("--provers");
	    // ogp -V | --version
	    version = args[0].equals("-V") || args[0].equals("--version");
	    if (!help && !provers && !version) {
		// ogp [CONJECTURE]
		theConjecture(args[0], configuration);
		theProverId(configuration.proverForExt(conjExt),
			    configuration);
		theProverArgs(args, 0);
	    }
	    break;
	case 2:
	    if (args[0].startsWith("--timeout=")) {
		// ogp --timeout=<time> [CONJECTURE]
		theTimeout(args[0].substring(10));
		theConjecture(args[1], configuration);
		theProverId(configuration.proverForExt(conjExt),
			    configuration);
		theProverArgs(args, 0);
	    } else {
		// ogp [CONJECTURE] [PROVER]
		theConjecture(args[0], configuration);
		theProverId(args[1], configuration);
		theProverArgs(args, 0);
		canProverProve(configuration);
	    }
	    break;
	case 3:
	    if (args[0].equals("-t")) {
		// ogp -t <time> [CONJECTURE]
		theTimeout(args[1]);
		theConjecture(args[2], configuration);
		theProverId(configuration.proverForExt(conjExt),
			    configuration);
		theProverArgs(args, 0);
	    } else if (args[0].startsWith("--timeout=")) {
		// ogp --timeout=<time> [CONJECTURE] [PROVER]
		theTimeout(args[0].substring(10));
		theConjecture(args[1], configuration);
		theProverId(args[2], configuration);
		theProverArgs(args, 0);
		canProverProve(configuration);
	    } else {
		// ogp [CONJECTURE] [PROVER] [PROVER-OPTIONS]
		theConjecture(args[0], configuration);
		theProverId(args[1], configuration);
		theProverArgs(args, 2);
		canProverProve(configuration);
	    }
	    break;
	case 4:
	    if (args[0].equals("-t")) {
		// ogp -t <time> [CONJECTURE] [PROVER]
		theTimeout(args[1]);
		theConjecture(args[2], configuration);
		theProverId(args[3], configuration);
		theProverArgs(args, 0);
		canProverProve(configuration);
	    } else if (args[0].startsWith("--timeout=")) {
		// ogp --timeout=<time> [CONJECTURE] [PROVER] [PROVER-OPTIONS]
		theTimeout(args[0].substring(10));
		theConjecture(args[1], configuration);
		theProverId(args[2], configuration);
		theProverArgs(args, 3);
		canProverProve(configuration);
	    } else {
		// ogp [CONJECTURE] [PROVER] [PROVER-OPTIONS]
		theConjecture(args[0], configuration);
		theProverId(args[1], configuration);
		theProverArgs(args, 2);
		canProverProve(configuration);
	    }
	    break;
	default:
	    if (args[0].equals("-t")) {
		// ogp -t <time> [CONJECTURE] [PROVER] [PROVER-OPTIONS]
		theTimeout(args[1]);
		theConjecture(args[2], configuration);
		theProverId(args[3], configuration);
		theProverArgs(args, 4);
		canProverProve(configuration);
	    } else if (args[0].startsWith("--timeout=")) {
		// ogp --timeout=<time> [CONJECTURE] [PROVER] [PROVER-OPTIONS]
		theTimeout(args[0].substring(10));
		theConjecture(args[1], configuration);
		theProverId(args[2], configuration);
		theProverArgs(args, 3);
		canProverProve(configuration);
	    } else {
		// ogp [CONJECTURE] [PROVER] [PROVER-OPTIONS]
		theConjecture(args[0], configuration);
		theProverId(args[1], configuration);
		theProverArgs(args, 2);
		canProverProve(configuration);
	    }
	    break;
	}
    }

    public int getTimeout() {
	return this.timeout;
    }
	    
    public String getConjName() {
	return this.conjName;
    }
    
    public String getConjExt() {
	return this.conjExt;
    }

    public String getProverId() {
	return this.proverId;
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

    private void canProverProve(OGPConf configuration) {
	if (!conjExt.equals(configuration.proverExt(proverId))) {
	    toFOFCmd = configuration
		.proverInfo(configuration.proverForExt(conjExt)).getToFOFCmd();
	    if (toFOFCmd.isEmpty()) {
		errorMsg(209, "");
	    }
	}
    }

    private void theConjecture(String conjecture, OGPConf configuration) {
	tgtp = conjecture.startsWith("--tgtp=");
	if (tgtp) {
	    if (conjecture.substring(7).isEmpty()) {
		errorMsg(202, "");
	    }
	    // TODO: Check if conjecture exists in TGTP
	    System.out.println("[OGP] Obtaining conjcture from TGTP not yet implemented.");
	    System.exit(0);
	} else {
	    File fileConjecture = new File(conjecture);
	    if (!fileConjecture.exists()) {
		errorMsg(203, conjecture);
	    } else if (!fileConjecture.isFile()) {
		errorMsg(204, conjecture);
	    } else if (!fileConjecture.canRead()) {
		errorMsg(205, conjecture);
	    } else {
		int index = conjecture.lastIndexOf(".");
		if (index == -1) {
		    errorMsg(206, conjecture);
		}
		conjName = conjecture.substring(0, conjecture.lastIndexOf('.'));
		conjExt = conjecture.substring(index + 1);
		// Possibly wrong...
		// if (!configuration.isExtAvailable(conjExt)) {
		//     errorMsg(207, conjExt);
		// }
	    }
	}
    }

    private void theProverArgs(String[] args, int position) {
	if (position == 0) {
	    proverArgs = "";
	} else {
	    StringBuilder theArgs = new StringBuilder(args[position]);
	    for (int i = position + 1; i < args.length; i++) {
		theArgs.append(" " + args[i]);
	    }
	    proverArgs = theArgs.toString();
	}
    }

    private void theProverId(String prover, OGPConf configuration) {
	if (!configuration.isProverAvailable(prover)) {
	    errorMsg(208, prover);
	} else {
	    proverId = prover;
	}
    }

    private void theTimeout(String time) {
	try {
	    timeout = Integer.parseInt(time);
	    if (timeout < 1) {
		errorMsg(210, "");
	    }
	} catch (NumberFormatException e) {
	    errorMsg(210, "");
	}
    }

    private static void errorMsg(int error, String msg) {
	System.err.print("[OGP ERROR " + error + "] (OGPArgs) ");
	switch (error) {
	case 201:
	    System.err.print("Incorrect number of arguments.");
	    System.err.println(" Use option '-h' for help.");
	    break;
	case 202:
	    System.err.println("Empty TGTP conjecture.");
	    break;
	case 203:
	    System.err.println("File '" + msg + "' does not exist.");
	    break;
	case 204:
	    System.err.println("'" + msg + "' is not a file.");
	    break;
	case 205:
	    System.err.println("Cannot read file '" + msg + "'.");
	    break;
	case 206:
	    System.err.print("Unable to determine the conjecture's format ");
	    System.err.println("from conjecture '" + msg + "'.");
	    break;
	// See "Possibly wrong..." comment in theConjecture() method.
	// case 207:
	//     System.err.println("Unrecognized extension '" + msg + "'.");
	//     break;
	case 208:
	    System.err.println("Unrecognized prover '" + msg + "'.");
	    break;
	case 209:
	    System.err.println("Unable to use prover on conjecture");
	    break;
	case 210:
	    System.err.println("Timeout must be a positive integer.");
	    break;
	}
	System.exit(error);
    }

}
