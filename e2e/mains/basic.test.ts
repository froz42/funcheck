import buildAndRun, { OUTPUT_NAME } from '../src/build-and-run';
import { FuncheckType } from '../src/types/funcheck.type';

const FILE_NAME = 'mains/basic.c';

describe(FILE_NAME, () => {
  const output = buildAndRun([FILE_NAME], {});

  it('should be a valid json', () => {
    expect(() => JSON.parse(output)).not.toThrow();
  });

  const outputObject: FuncheckType = JSON.parse(output);

  describe('outputObject', () => {
    it('should have a valid version', () => {
      expect(outputObject['funcheck-version']).toBeDefined();
    });

    it('should have a valid program name', () => {
      expect(outputObject.program).toBe(OUTPUT_NAME);
    });

    it('should have a valid arguments', () => {
      expect(outputObject.arguments).toEqual([]);
    });
  });

  describe('function fetch', () => {
    const functionFetch = outputObject['function-fetch'];

    it('should be defined', () => {
      expect(functionFetch).toBeDefined();
    });

    it('should have a valid output', () => {
      expect(functionFetch.output).toBe("Hello world\n")
    });

    it('should have a valid return code', () => {
      expect(functionFetch['return-code']).toBe(0);
    });

    it('should have a valid function detected count', () => {
      expect(functionFetch['function-detected-count']).toBe(1);
    });
  });

  describe('function tests', () => {
    const functionTests = outputObject['function-tests'];

    it('should be defined', () => {
      expect(functionTests).toBeDefined();
    });

    it('should have a length of 1', () => {
      expect(functionTests.length).toBe(1);
    });

    describe('function test', () => {
      const functionTest = functionTests[0];

      it('should have a valid function name', () => {
        expect(functionTest['function-name']).toBe('malloc');
      });

      it('should have a valid return code', () => {
        expect(functionTest['return-code']).toBe(1);
      });

      it('should have a valid function backtrace', () => {
        expect(functionTest['function-backtrace']).toBeDefined();
        
        const functionBacktrace = functionTest['function-backtrace'];
        expect(functionBacktrace.length).toBe(1);
        
        const backtrace = functionBacktrace[0];
        expect(backtrace.address).toBeDefined();
        expect(backtrace.function).toBe('main');
        expect(backtrace.file).toBe(FILE_NAME);
        expect(backtrace.line).toBe(7);
        expect(backtrace.column).toBe(17);
      });
    });
  });
});
