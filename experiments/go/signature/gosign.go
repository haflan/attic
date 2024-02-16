package main

import (
	"fmt"
	"os"

	"golang.org/x/crypto/ssh"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("no ssh private key given")
		os.Exit(1)
	}
	if len(os.Args) < 3 {
		fmt.Println("no file given")
		os.Exit(1)
	}
	if len(os.Args) < 4 {
		fmt.Println("no ssh public key given")
		os.Exit(1)
	}

	// private key and sign
	privKey, err := os.ReadFile(os.Args[1])
	if err != nil {
		panic(fmt.Sprintln("1", err))
	}
	sshPriv, err := ssh.ParsePrivateKey(privKey)
	if err != nil {
		panic(fmt.Sprintln("2", err))
	}
	contents, err := os.ReadFile(os.Args[2])
	if err != nil {
		panic(fmt.Sprintln("3", err))
	}
	sig, err := sshPriv.Sign(nil, contents)
	if err != nil {
		panic(fmt.Sprintln("4", err))
	}

	// fmt.Printf("%v %x %x\n", sig.Format, sig.Blob, sig.Rest)

	// load public key and verify
	pubKey, err := os.ReadFile(os.Args[3])
	if err != nil {
		panic(fmt.Sprintln("5", err))
	}
	sshPub, err := ssh.ParsePublicKey(pubKey)
	if err != nil {
		panic(fmt.Sprintln("6", err))
	}
	err = sshPub.Verify(contents, sig)
	if err != nil {
		panic(fmt.Sprintln("7", err))
	}
}
