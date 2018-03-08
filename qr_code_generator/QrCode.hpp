/* 
 * QR Code generator library (C++)
 * 
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "QrSegment.hpp"


namespace qrcodegen {

/* 
 * Represents an immutable square grid of black and white cells for a QR Code symbol, and
 * provides static functions to create a QR Code from user-supplied textual or binary data.
 * This class covers the QR Code model 2 specification, supporting all versions (sizes)
 * from 1 to 40, all 4 error correction levels, and only 3 character encoding modes.
 */
class QrCode final {
	
	/*---- Public helper enumeration ----*/
	
	/* 
	 * Represents the error correction level used in a QR Code symbol.
	 */
	public: enum class Ecc {
		// Constants declared in ascending order of error protection.
		LOW = 0, MEDIUM = 1, QUARTILE = 2, HIGH = 3
	};
	
	
	// Returns a value in the range 0 to 3 (unsigned 2-bit integer).
	private: static int getFormatBits(Ecc ecl);
	
	
	
	/*---- Public static factory functions ----*/
	
	/* 
	 * Returns a QR Code symbol representing the specified Unicode text string at the specified error correction level.
	 * As a conservative upper bound, this function is guaranteed to succeed for strings that have 2953 or fewer
	 * UTF-8 code units (not Unicode code points) if the low error correction level is used. The smallest possible
	 * QR Code version is automatically chosen for the output. The ECC level of the result may be higher than
	 * the ecl argument if it can be done without increasing the version.
	 */
	public: static QrCode encodeText(const char *text, Ecc ecl);
	
	
	/* 
	 * Returns a QR Code symbol representing the given binary data string at the given error correction level.
	 * This function always encodes using the binary segment mode, not any text mode. The maximum number of
	 * bytes allowed is 2953. The smallest possible QR Code version is automatically chosen for the output.
	 * The ECC level of the result may be higher than the ecl argument if it can be done without increasing the version.
	 */
	public: static QrCode encodeBinary(const std::vector<std::uint8_t> &data, Ecc ecl);
	
	
	/* 
	 * Returns a QR Code symbol representing the given data segments with the given encoding parameters.
	 * The smallest possible QR Code version within the given range is automatically chosen for the output.
	 * This function allows the user to create a custom sequence of segments that switches
	 * between modes (such as alphanumeric and binary) to encode text more efficiently.
	 * This function is considered to be lower level than simply encoding text or binary data.
	 */
	public: static QrCode encodeSegments(const std::vector<QrSegment> &segs, Ecc ecl,
		int minVersion=1, int maxVersion=40, int mask=-1, bool boostEcl=true);  // All optional parameters
	
	
	
	/*---- Public constants ----*/
	
	public: static constexpr int MIN_VERSION =  1;
	public: static constexpr int MAX_VERSION = 40;
	
	
	
	/*---- Instance fields ----*/
	
	// Immutable scalar parameters
	
	/* This QR Code symbol's version number, which is always between 1 and 40 (inclusive). */
	private: int version;
	
	/* The width and height of this QR Code symbol, measured in modules.
	 * Always equal to version &times; 4 + 17, in the range 21 to 177. */
	private: int size;
	
	/* The error correction level used in this QR Code symbol. */
	private: Ecc errorCorrectionLevel;
	
	/* The mask pattern used in this QR Code symbol, in the range 0 to 7 (i.e. unsigned 3-bit integer).
	 * Note that even if a constructor was called with automatic masking requested
	 * (mask = -1), the resulting object will still have a mask value between 0 and 7. */
	private: int mask;
	
	// Private grids of modules/pixels (conceptually immutable)
	private: std::vector<std::vector<bool> > modules;     // The modules of this QR Code symbol (false = white, true = black)
	private: std::vector<std::vector<bool> > isFunction;  // Indicates function modules that are not subjected to masking
	
	
	
	/*---- Constructors ----*/
	
	/* 
	 * Creates a new QR Code symbol with the given version number, error correction level, binary data array,
	 * and mask number. This is a cumbersome low-level constructor that should not be invoked directly by the user.
	 * To go one level up, see the encodeSegments() function.
	 */
	public: QrCode(int ver, Ecc ecl, const std::vector<std::uint8_t> &dataCodewords, int mask);
	
	
	
	/*---- Public instance methods ----*/
	
	public: int getVersion() const;
	
	
	public: int getSize() const;
	
	
	public: Ecc getErrorCorrectionLevel() const;
	
	
	public: int getMask() const;
	
	
	/* 
	 * Returns the color of the module (pixel) at the given coordinates, which is either
	 * false for white or true for black. The top left corner has the coordinates (x=0, y=0).
	 * If the given coordinates are out of bounds, then false (white) is returned.
	 */
	public: bool getModule(int x, int y) const;
	
	
	/* 
	 * Based on the given number of border modules to add as padding, this returns a
	 * string whose contents represents an SVG XML file that depicts this QR Code symbol.
	 * Note that Unix newlines (\n) are always used, regardless of the platform.
	 */
	public: std::string toSvgString(int border) const;
	
	
	
	/*---- Private helper methods for constructor: Drawing function modules ----*/
	
	private: void drawFunctionPatterns();
	
	
	// Draws two copies of the format bits (with its own error correction code)
	// based on the given mask and this object's error correction level field.
	private: void drawFormatBits(int mask);
	
	
	// Draws two copies of the version bits (with its own error correction code),
	// based on this object's version field (which only has an effect for 7 <= version <= 40).
	private: void drawVersion();
	
	
	// Draws a 9*9 finder pattern including the border separator, with the center module at (x, y).
	private: void drawFinderPattern(int x, int y);
	
	
	// Draws a 5*5 alignment pattern, with the center module at (x, y).
	private: void drawAlignmentPattern(int x, int y);
	
	
	// Sets the color of a module and marks it as a function module.
	// Only used by the constructor. Coordinates must be in range.
	private: void setFunctionModule(int x, int y, bool isBlack);
	
	
	// Returns the color of the module at the given coordinates, which must be in range.
	private: bool module(int x, int y) const;
	
	
	/*---- Private helper methods for constructor: Codewords and masking ----*/
	
	// Returns a new byte string representing the given data with the appropriate error correction
	// codewords appended to it, based on this object's version and error correction level.
	private: std::vector<std::uint8_t> appendErrorCorrection(const std::vector<std::uint8_t> &data) const;
	
	
	// Draws the given sequence of 8-bit codewords (data and error correction) onto the entire
	// data area of this QR Code symbol. Function modules need to be marked off before this is called.
	private: void drawCodewords(const std::vector<std::uint8_t> &data);
	
	
	// XORs the data modules in this QR Code with the given mask pattern. Due to XOR's mathematical
	// properties, calling applyMask(m) twice with the same value is equivalent to no change at all.
	// This means it is possible to apply a mask, undo it, and try another mask. Note that a final
	// well-formed QR Code symbol needs exactly one mask applied (not zero, not two, etc.).
	private: void applyMask(int mask);
	
	
	// A messy helper function for the constructors. This QR Code must be in an unmasked state when this
	// method is called. The given argument is the requested mask, which is -1 for auto or 0 to 7 for fixed.
	// This method applies and returns the actual mask chosen, from 0 to 7.
	private: int handleConstructorMasking(int mask);
	
	
	// Calculates and returns the penalty score based on state of this QR Code's current modules.
	// This is used by the automatic mask choice algorithm to find the mask pattern that yields the lowest score.
	private: long getPenaltyScore() const;
	
	
	
	/*---- Private static helper functions ----*/
	
	// Returns a set of positions of the alignment patterns in ascending order. These positions are
	// used on both the x and y axes. Each value in the resulting array is in the range [0, 177).
	// This stateless pure function could be implemented as table of 40 variable-length lists of unsigned bytes.
	private: static std::vector<int> getAlignmentPatternPositions(int ver);
	
	
	// Returns the number of data bits that can be stored in a QR Code of the given version number, after
	// all function modules are excluded. This includes remainder bits, so it might not be a multiple of 8.
	// The result is in the range [208, 29648]. This could be implemented as a 40-entry lookup table.
	private: static int getNumRawDataModules(int ver);
	
	
	// Returns the number of 8-bit data (i.e. not error correction) codewords contained in any
	// QR Code of the given version number and error correction level, with remainder bits discarded.
	// This stateless pure function could be implemented as a (40*4)-cell lookup table.
	private: static int getNumDataCodewords(int ver, Ecc ecl);
	
	
	/*---- Private tables of constants ----*/
	
	// For use in getPenaltyScore(), when evaluating which mask is best.
	private: static const int PENALTY_N1;
	private: static const int PENALTY_N2;
	private: static const int PENALTY_N3;
	private: static const int PENALTY_N4;
	
	private: static const std::int8_t ECC_CODEWORDS_PER_BLOCK[4][41];
	private: static const std::int8_t NUM_ERROR_CORRECTION_BLOCKS[4][41];
	
	
	
	/*---- Private helper class ----*/
	
	/* 
	 * Computes the Reed-Solomon error correction codewords for a sequence of data codewords
	 * at a given degree. Objects are immutable, and the state only depends on the degree.
	 * This class exists because each data block in a QR Code shares the same the divisor polynomial.
	 */
	private: class ReedSolomonGenerator final {
		
		/*-- Immutable field --*/
		
		// Coefficients of the divisor polynomial, stored from highest to lowest power, excluding the leading term which
		// is always 1. For example the polynomial x^3 + 255x^2 + 8x + 93 is stored as the uint8 array {255, 8, 93}.
		private: std::vector<std::uint8_t> coefficients;
		
		
		/*-- Constructor --*/
		
		/* 
		 * Creates a Reed-Solomon ECC generator for the given degree. This could be implemented
		 * as a lookup table over all possible parameter values, instead of as an algorithm.
		 */
		public: explicit ReedSolomonGenerator(int degree);
		
		
		/*-- Method --*/
		
		/* 
		 * Computes and returns the Reed-Solomon error correction codewords for the given
		 * sequence of data codewords. The returned object is always a new byte array.
		 * This method does not alter this object's state (because it is immutable).
		 */
		public: std::vector<std::uint8_t> getRemainder(const std::vector<std::uint8_t> &data) const;
		
		
		/*-- Static function --*/
		
		// Returns the product of the two given field elements modulo GF(2^8/0x11D).
		// All inputs are valid. This could be implemented as a 256*256 lookup table.
		private: static std::uint8_t multiply(std::uint8_t x, std::uint8_t y);
		
	};
	
};

}
