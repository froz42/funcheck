import buildAndRun, { OUTPUT_NAME } from '../src/build-and-run';
import { FuncheckType } from '../src/types/funcheck.type';

const FILE_NAME = 'mains/crashing.c';

const EXIT_SEGV = 139;

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
      expect(functionFetch.output).toBeDefined();
    });

    it('should have a valid return code', () => {
      expect(functionFetch['return-code']).toBe(EXIT_SEGV);
    });

    it('should have a valid function detected count', () => {
      expect(functionFetch['function-detected-count']).toBe(0);
    });

    describe('crash', () => {
        const crash = functionFetch.crash;
    
        it('should be defined', () => {
            expect(crash).toBeDefined();
        });
    
        it('should have a length of 1', () => {
            expect(crash?.name).toBe('Segmentation fault');
        });

        describe('backtrace', () => {
            const backtrace = crash?.backtrace;
        
            it('should be defined', () => {
                expect(backtrace).toBeDefined();
            });
        
            it('should have a length of 1', () => {
                expect(backtrace?.length).toBe(1);
            });

            describe('backtrace item', () => {
                const backtraceItem = backtrace?.[0];
            
                it('should be defined', () => {
                    expect(backtraceItem).toBeDefined();
                });
            
                it('should have a valid address', () => {
                    expect(backtraceItem?.address).toBeDefined();
                });
            
                it('should have a valid function', () => {
                    expect(backtraceItem?.function).toBe('main');
                });
            
                it('should have a valid file', () => {
                    expect(backtraceItem?.file).toBe(FILE_NAME);
                });
            
                it('should have a valid line', () => {
                    expect(backtraceItem?.line).toBe(8);
                });
            
                it('should have a valid column', () => {
                    expect(backtraceItem?.column).toBe(5);
                });
            });
        });
    });
  });
});
