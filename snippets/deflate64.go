// Compress and encode, or decode and decompress
// with base64 and deflate (zlib).
// This can be useful for sending any data as text,
// and it's the format used by kroki (https://kroki.io/).

package main

import (
	"bufio"
	"bytes"
	"compress/zlib"
	"encoding/base64"
	"fmt"
	"io"
	"os"
)

// Encode takes a string and returns an encoded string in deflate + base64 format
// (stolen from https://docs.kroki.io/kroki/setup/encode-diagram/#go)

func Encode(input string) (string, error) {
	var buffer bytes.Buffer
	writer, err := zlib.NewWriterLevel(&buffer, 9)
	if err != nil {
		return "", fmt.Errorf("failed to create the writer: %w", err)
	}
	_, err = writer.Write([]byte(input))
	writer.Close()
	if err != nil {
		return "", fmt.Errorf("fail to create the payload: %w", err)
	}
	result := base64.URLEncoding.EncodeToString(buffer.Bytes())
	return result, nil
}

func Decode(zippedB64 string) (string, error) {
	result, err := base64.URLEncoding.DecodeString(zippedB64)
	if err != nil {
		return "", fmt.Errorf("failed to decode base64: %w", err)
	}
	rbuf := bytes.NewReader([]byte(result))
	r, err := zlib.NewReader(rbuf)
	if err != nil {
		return "", fmt.Errorf("failed to create the reader: %w", err)
	}
	var buf bytes.Buffer
	_, err = io.Copy(bufio.NewWriter(&buf), r)
	return buf.String(), err
}

func main() {
	if len(os.Args) < 3 {
		fmt.Printf("Use: %v [encode|decode] <input>", os.Args[0])
		os.Exit(1)
	}
	var result string
	var err error
	switch os.Args[1] {
	case "decode":
		result, err = Decode(os.Args[2])
	default:
		result, err = Encode(os.Args[2])
	}
	if err != nil {
		fmt.Println("error:", err)
	} else {
		fmt.Println(result)
	}
}
