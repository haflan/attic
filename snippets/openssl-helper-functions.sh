#!/bin/bash

# OpenSSL reference and helper functions for dealing with certificates.

# Source this in the terminal (`. openssl-helper-functions.sh`) and run the functions
# as desired, or for dev purposes, run  `bash openssl-helper-functions.sh runall` to
# generate a new CA and signed server certificates.


# This is pretty much just to demonstrate all of the functions below
function run_all_cert_functions() {
    generate_ca          ca
    generate_private_key localhost
    generate_csr         localhost
    sign_csr             ca localhost
    bundle_certs         ca localhost
}

function generate_key_and_cert() {
    ca=$1
    server=$2
    generate_private_key $server
    generate_csr         $server
    sign_csr             $ca $server
    bundle_certs         $ca $server
}


# Generate CA
# Key is stored in $1.key, and self-signed certificate in $1.pem
function generate_ca() {
    name=$1
    openssl genrsa -out $name.key 2048
    openssl req -new -x509 \
        -key $name.key \
        -out $name.pem \
        -days 18000 \
        -subj "/C=NO/ST=Viken/L=Drammen"
}

# Generate new private key
# Saved to $1.key
function generate_private_key() {
    name=$1
    openssl genrsa -out $name.key 2048
}

# Generate Certificate Signing Request
# Assumes that $1.key exists in the working directory
function generate_csr() {
    name=$1
    subject="/C=NO/ST=Viken/L=Drammen/CN=$1"
    openssl req -new \
        -key $name.key \
        -out $name.csr \
        -subj "$subject"
}

# Sign the certificate
# $1 = CA name, $2 = Server / client name
# Assumes that all files, i.e. CSR, CA cert and key, exist in the working directory
function sign_csr() {
    ca=$1
    server=$2
    openssl x509 -req \
        -days 3600 \
        -CA $ca.pem \
        -CAkey $ca.key \
        -CAcreateserial \
        -in $server.csr \
        -out $server.pem \
    # CSR shouldn't be needed again, *I think*:
    rm $server.csr 
}

# Bundle certificates with key in PKCS12 format
# $1 = CA name, $2 = Server / client name
# Userful in Java code, among other things
function bundle_certs() {
    ca=$1
    server=$2
    echo "Please enter password for .p12 file"
    echo "This is the password you will probably use in your Java code"
    openssl pkcs12 -export \
        -certfile $ca.pem \
        -in $server.pem \
        -inkey $server.key \
        -out $server.p12
}

function get_server_cert() {
    server=$1
    output=$2
    openssl s_client -connect $server -showcerts 2>/dev/null </dev/null > $output
}

function ks_list() {
    keystore=$1
    keytool -list -keystore $keystore
}

# Extract PEM from a Java keystore entry
function ks_alias_to_pem() {
    keystore=$1
    alias=$2
    keytool -exportcert \
        -keystore $keystore \
        -alias $alias | openssl x509 -inform der -text > $alias.pem
}

# (untested)
# Convert entire Java keystore to PEM file with all keys and certificates
function ks_to_pem() {
    keystore=$1
    target=$2
    # Convert JKS to PKCS12
    keytool -importkeystore -srckeystore $keystore \
        -destkeystore $target.p12 \
        -srcstoretype jks \
        -deststoretype pkcs12
    p12_to_pem $target.p12
}


function pem_to_p12_cert() {
    if [ -z "$1" ]; then
        echo "pem_to_p12_cert:"
        echo "Converts PEM certificate file to a PKCS12 file."
	echo "Most relevant for creating a .p12 truststore with the certificates."
        echo "Usage:"
	echo "	pem_to_p12_cert <pem file>"
	return
    fi
    pem_file=$1
    pkcs_file=$pem_file.p12
    openssl pkcs12 -export -nokeys -in $pem_file -out $pkcs_file
}

function p12_to_pem() {
    if [ -z "$1" ]; then
        echo "p12_to_pem:"
        echo "Converts PKCS12 file to a single textual PEM file."
	echo "Generated file is named <pkcs_file>.pem"
        echo "If there are any keys in the PKCS file, they will stay encrypted."
        echo "Usage:"
	echo "	p12_to_pem <pkcs_file>"
	return
    fi
    pkcs_file=$1
    pem_file=$1.pem # <-- TODO: improve
    # Create PEM with all info from P12
    openssl pkcs12 -in $pkcs_file -out $pem_file -nodes
}

function p12_extract_cert() {
    pkcs_file=$1
    cert_file=$1.crt
    # Convert PKCS12 to PEM *without encryption (omit -nodes to encrypt)
    openssl pkcs12 -in $pkcs_file -out $cert_file -nodes -nokeys
}

function p12_extract_rsa_key() {
    if [ -z "$1" ]; then
        echo "p12_extract_rsa_key:"
        echo "Extracts and decrypts an RSA key from a PKCS12 formatted file"
	echo "Generated file is named <pkcs_file>.key"
        echo "Usage:"
	echo "	p12_extract_rsa_key <pkcs_file>"
        echo
	return
    fi
    pkcs_file=$1
    key_file=$1.key
    openssl pkcs12 -in $pkcs_file -nodes -nocerts | openssl rsa -out $key_file
}

function verify_signed_cert() {
    if [ -z "$2" ]; then
        echo "verify_signed_cert:"
	echo "Verifies the given certificate against the given CA certificate(s)"
        echo "Usage:"
	echo "	verify_signed_cert <trusted_ca> [<intermediate_ca_bundle>] <certificate>"
        echo
	return
    fi
    if [ -z "$3" ]; then
        ca_cert=$1
        cert=$2
    else
        ca_cert=$1
        untrusted="-untrusted $2" # Intermediate cert(s)
        cert=$3
    fi
    openssl verify --CAfile $ca_cert $untrusted $cert

}

if [ "$1" == "runall" ]; then 
    run_all_cert_functions
fi
