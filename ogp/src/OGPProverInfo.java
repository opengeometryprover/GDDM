package ogp;

public class OGPProverInfo {

    private String id;		// Prover's name
    private String cmd;		// Command line command
    private String ext;		// Conjecture's extension
    private String toFOFCmd;	// ext2FOF command, if any
    private String toExtCmd;	// FOF2ext command, if any
    private String postProcCmd;	// Post-processing command, if any
    private String desc;	// Prover's description

    public OGPProverInfo(String id,
			 String cmd,
			 String ext,
			 String toFOFCmd,
			 String toExtCmd,
			 String postProcCmd,
			 String desc) {
	this.id = id;
	this.cmd = cmd;
	this.ext = ext;
	this.toFOFCmd = toFOFCmd;
	this.toExtCmd = toExtCmd;
	this.postProcCmd = postProcCmd;
	this.desc = desc;
    }

    public String getId() {
	return this.id;
    }

    public String getCmd() {
	return this.cmd;
    }

    public String getExt() {
	return this.ext;
    }

    public String getToFOFCmd() {
	return this.toFOFCmd;
    }

    public String getToExtCmd() {
	return this.toExtCmd;
    }

    public String getPostProcCmd() {
	return this.postProcCmd;
    }

    public String getDesc() {
	return this.desc;
    }

}
